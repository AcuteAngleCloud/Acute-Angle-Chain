/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */
#include <aacio/wallet_plugin/wallet_manager.hpp>
#include <aacio/wallet_plugin/wallet.hpp>
#include <aacio/wallet_plugin/se_wallet.hpp>
#include <aacio/chain/exceptions.hpp>
#include <aacio/chain_plugin/chain_plugin.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <fc/io/json.hpp>
#include <regex>
#include "httpc.hpp"
#include "localize.hpp"

using namespace aacio::client::http;
using namespace aacio::client::localize;

std::string url = "http://127.0.0.1:8888/";
aacio::client::http::http_context context;

namespace aacio {
namespace wallet {

constexpr auto file_ext = ".wallet";
constexpr auto password_prefix = "PW";
bool no_verify = false; //don't verify peer certificate when using HTTPS
std::vector<std::string> headers;
auto   tx_expiration = fc::seconds(30);
std::string tx_ref_block_num_or_id;
bool   tx_force_unique = false;
bool   tx_skip_sign = false;

uint8_t  tx_max_cpu_usage = 0;
uint32_t tx_max_net_usage = 0;

std::string gen_password() {
   auto key = private_key_type::generate();
   return password_prefix + string(key);

}

bool valid_filename(const string& name) {
   if (name.empty()) return false;
   if (std::find_if(name.begin(), name.end(), !boost::algorithm::is_alnum() && !boost::algorithm::is_any_of("._-")) != name.end()) return false;
   return boost::filesystem::path(name).filename().string() == name;
}

bool valid_waltpin(const string &walt_pin)
{
   return (walt_pin.length() > 5 && walt_pin.length() < 19);
}

wallet_manager::wallet_manager() {
#ifdef __APPLE__
   try {
      wallets.emplace("SecureEnclave", std::make_unique<se_wallet>());
   } catch(fc::exception& ) {}
#endif
}

void wallet_manager::set_timeout(const std::chrono::seconds& t) {
   timeout = t;
   auto now = std::chrono::system_clock::now();
   timeout_time = now + timeout;
   AAC_ASSERT(timeout_time >= now, invalid_lock_timeout_exception, "Overflow on timeout_time, specified ${t}, now ${now}, timeout_time ${timeout_time}",
             ("t", t.count())("now", now.time_since_epoch().count())("timeout_time", timeout_time.time_since_epoch().count()));
}

void wallet_manager::check_timeout() {
   if (timeout_time != timepoint_t::max()) {
      const auto& now = std::chrono::system_clock::now();
      if (now >= timeout_time) {
         lock_all();
      }
      timeout_time = now + timeout;
   }
}

void wallet_manager::create(const std::string& name, const std::string& walt_pin) {
   check_timeout();

   AAC_ASSERT(valid_filename(name), wallet_exception, "Invalid filename, path not allowed in wallet name ${n}", ("n", name));

   AAC_ASSERT(valid_waltpin(walt_pin), wallet_exception, "Invalid wallet password, the password length must be between 6 and 18 characters long");

   auto wallet_filename = dir / (name + file_ext);

   if (fc::exists(wallet_filename)) {
      AAC_THROW(chain::wallet_exist_exception, "Wallet with name: '${n}' already exists at ${path}", ("n", name)("path",fc::path(wallet_filename)));
   }

   std::string password = walt_pin;
   wallet_data d;
   auto wallet = make_unique<soft_wallet>(d);
   wallet->set_password(password);
   wallet->set_wallet_filename(wallet_filename.string());
   wallet->unlock(password);
   wallet->lock();
   wallet->unlock(password);

   // Explicitly save the wallet file here, to ensure it now exists.
   wallet->save_wallet_file();

   // If we have name in our map then remove it since we want the emplace below to replace.
   // This can happen if the wallet file is removed while aac-walletd is running.
   auto it = wallets.find(name);
   if (it != wallets.end()) {
      wallets.erase(it);
   }
   wallets.emplace(name, std::move(wallet));

}

void wallet_manager::open(const std::string& name) {
   check_timeout();

   AAC_ASSERT(valid_filename(name), wallet_exception, "Invalid filename, path not allowed in wallet name ${n}", ("n", name));

   wallet_data d;
   auto wallet = std::make_unique<soft_wallet>(d);
   auto wallet_filename = dir / (name + file_ext);
   wallet->set_wallet_filename(wallet_filename.string());
   if (!wallet->load_wallet_file()) {
      AAC_THROW(chain::wallet_nonexistent_exception, "Unable to open file: ${f}", ("f", wallet_filename.string()));
   }

   // If we have name in our map then remove it since we want the emplace below to replace.
   // This can happen if the wallet file is added while aac-walletd is running.
   auto it = wallets.find(name);
   if (it != wallets.end()) {
      wallets.erase(it);
   }
   wallets.emplace(name, std::move(wallet));
}

std::vector<std::string> wallet_manager::list_wallets() {
   check_timeout();
   std::vector<std::string> result;
   for (const auto& i : wallets) {
      if (i.second->is_locked()) {
         result.emplace_back(i.first);
      } else {
         result.emplace_back(i.first + " *");
      }
   }
   return result;
}

map<public_key_type,private_key_type> wallet_manager::list_keys(const string& name, const string& pw) {
   check_timeout();

   if (wallets.count(name) == 0)
      AAC_THROW(chain::wallet_nonexistent_exception, "Wallet not found: ${w}", ("w", name));
   auto& w = wallets.at(name);
   if (w->is_locked())
      AAC_THROW(chain::wallet_locked_exception, "Wallet is locked: ${w}", ("w", name));
   w->check_password(pw); //throws if bad password
   return w->list_keys();
}

flat_set<public_key_type> wallet_manager::get_public_keys() {
   check_timeout();
   AAC_ASSERT(!wallets.empty(), wallet_not_available_exception, "You don't have any wallet!");
   flat_set<public_key_type> result;
   bool is_all_wallet_locked = true;
   for (const auto& i : wallets) {
      if (!i.second->is_locked()) {
         result.merge(i.second->list_public_keys());
      }
      is_all_wallet_locked &= i.second->is_locked();
   }
   AAC_ASSERT(!is_all_wallet_locked, wallet_locked_exception, "You don't have any unlocked wallet!");
   return result;
}


void wallet_manager::lock_all() {
   // no call to check_timeout since we are locking all anyway
   for (auto& i : wallets) {
      if (!i.second->is_locked()) {
         i.second->lock();
      }
   }
}

void wallet_manager::lock(const std::string& name) {
   check_timeout();
   if (wallets.count(name) == 0) {
      AAC_THROW(chain::wallet_nonexistent_exception, "Wallet not found: ${w}", ("w", name));
   }
   auto& w = wallets.at(name);
   if (w->is_locked()) {
      return;
   }
   w->lock();
}

void wallet_manager::unlock(const std::string& name, const std::string& password) {
   check_timeout();
   if (wallets.count(name) == 0) {
      open( name );
   }
   auto& w = wallets.at(name);
   if (!w->is_locked()) {
      AAC_THROW(chain::wallet_unlocked_exception, "Wallet is already unlocked: ${w}", ("w", name));
      return;
   }
   w->unlock(password);
}

void wallet_manager::import_key(const std::string& name, const std::string& wif_key) {
   check_timeout();
   if (wallets.count(name) == 0) {
      AAC_THROW(chain::wallet_nonexistent_exception, "Wallet not found: ${w}", ("w", name));
   }
   auto& w = wallets.at(name);
   if (w->is_locked()) {
      AAC_THROW(chain::wallet_locked_exception, "Wallet is locked: ${w}", ("w", name));
   }
   w->import_key(wif_key);
}

void wallet_manager::remove_key(const std::string& name, const std::string& password, const std::string& key) {
   check_timeout();
   if (wallets.count(name) == 0) {
      AAC_THROW(chain::wallet_nonexistent_exception, "Wallet not found: ${w}", ("w", name));
   }
   auto& w = wallets.at(name);
   if (w->is_locked()) {
      AAC_THROW(chain::wallet_locked_exception, "Wallet is locked: ${w}", ("w", name));
   }
   w->check_password(password); //throws if bad password
   w->remove_key(key);
}

string wallet_manager::create_key(const std::string& name, const std::string& key_type) {
   check_timeout();
   if (wallets.count(name) == 0) {
      AAC_THROW(chain::wallet_nonexistent_exception, "Wallet not found: ${w}", ("w", name));
   }
   auto& w = wallets.at(name);
   if (w->is_locked()) {
      AAC_THROW(chain::wallet_locked_exception, "Wallet is locked: ${w}", ("w", name));
   }

   string upper_key_type = boost::to_upper_copy<std::string>(key_type);
   return w->create_key(upper_key_type);
}

chain::signed_transaction
wallet_manager::sign_transaction(const chain::signed_transaction& txn, const flat_set<public_key_type>& keys, const chain::chain_id_type& id) {
   check_timeout();
   chain::signed_transaction stxn(txn);

   for (const auto& pk : keys) {
      bool found = false;
      for (const auto& i : wallets) {
         if (!i.second->is_locked()) {
            optional<signature_type> sig = i.second->try_sign_digest(stxn.sig_digest(id, stxn.context_free_data), pk);
            if (sig) {
               stxn.signatures.push_back(*sig);
               found = true;
               break; // inner for
            }
         }
      }
      if (!found) {
         AAC_THROW(chain::wallet_missing_pub_key_exception, "Public key not found in unlocked wallets ${k}", ("k", pk));
      }
   }

   return stxn;
}

chain::signature_type
wallet_manager::sign_digest(const chain::digest_type& digest, const public_key_type& key) {
   check_timeout();

   try {
      for (const auto& i : wallets) {
         if (!i.second->is_locked()) {
            optional<signature_type> sig = i.second->try_sign_digest(digest, key);
            if (sig)
               return *sig;
         }
      }
   } FC_LOG_AND_RETHROW();

   AAC_THROW(chain::wallet_missing_pub_key_exception, "Public key not found in unlocked wallets ${k}", ("k", key));
}

void wallet_manager::own_and_use_wallet(const string& name, std::unique_ptr<wallet_api>&& wallet) {
   if(wallets.find(name) != wallets.end())
      FC_THROW("tried to use wallet name the already existed");
   wallets.emplace(name, std::move(wallet));
}

void wallet_manager::change_password(const std::string& name, const std::string& password) {
   check_timeout();
   if (wallets.count(name) == 0) {
      AAC_THROW(chain::wallet_nonexistent_exception, "Wallet not found: ${w}", ("w", name));
   }
   auto& w = wallets.at(name);
   if (w->is_locked()) {
      AAC_THROW(chain::wallet_locked_exception, "Wallet is locked: ${w}", ("w", name));
   }
   AAC_ASSERT(valid_waltpin(password), wallet_exception, "Invalid wallet password, the password length must be between 6 and 18 characters long");

   w->set_password(password);
   w->unlock(password);
   w->save_wallet_file();
}

template<typename T>
fc::variant call( const std::string& url,
                  const std::string& path,
                  const T& v ) {
   try {
      aacio::client::http::connection_param *cp = new aacio::client::http::connection_param(context, parse_url(url) + path,
              no_verify ? false : true, headers);

      return aacio::client::http::do_http_call( *cp, fc::variant(v), false, false );
   }
   catch(boost::system::system_error& e) {
      if(url == ::url)
         std::cerr << localized("Failed to connect to nodaac at ${u}; is nodaac running?", ("u", url)) << std::endl;

      throw connection_exception(fc::log_messages{FC_LOG_MESSAGE(error, e.what())});
   }
}

template<typename T>
fc::variant call( const std::string& path,
                  const T& v ) { return call( url, path, fc::variant(v) ); }

template<>
fc::variant call( const std::string& url,
                  const std::string& path) { return call( url, path, fc::variant() ); }

aacio::chain_apis::read_only::get_info_results get_info() {
   return call(::url, get_info_func).as<aacio::chain_apis::read_only::get_info_results>();
}

chain::action generate_nonce_action() {
   return chain::action( {}, config::null_account_name, "nonce", fc::raw::pack(fc::time_point::now().time_since_epoch().count()));
}

bytes variant_to_bin( const account_name& account, const action_name& action, const fc::variant& action_args_var ) {
   static unordered_map<account_name, std::vector<char> > abi_cache;
   auto it = abi_cache.find( account );
   if ( it == abi_cache.end() ) {
      const auto result = call(get_raw_code_and_abi_func, fc::mutable_variant_object("account_name", account));
      std::tie( it, std::ignore ) = abi_cache.emplace( account, result["abi"].as_blob().data );
      //we also received result["wasm"], but we don't use it
   }
   const std::vector<char>& abi_v = it->second;

   abi_def abi;
   if( abi_serializer::to_abi(abi_v, abi) ) {
      abi_serializer abis( abi, fc::seconds(10) );
      auto action_type = abis.get_action_type(action);
      FC_ASSERT(!action_type.empty(), "Unknown action ${action} in contract ${contract}", ("action", action)("contract", account));
      return abis.variant_to_binary(action_type, action_args_var, fc::seconds(10));
   } else {
      FC_ASSERT(false, "No ABI found for ${contract}", ("contract", account));
   }
}

fc::variant wallet_manager::determine_required_keys(const chain::signed_transaction& trx) {
   // TODO better error checking
   //wdump((trx));
   const flat_set<public_key_type>& public_keys = get_public_keys();
   auto get_arg = fc::mutable_variant_object
           ("transaction", (chain::transaction)trx)
           ("available_keys", public_keys);
   const auto& required_keys = call(get_required_keys, get_arg);
   return required_keys["required_keys"];
}

fc::variant wallet_manager::push_transaction( chain::signed_transaction& trx, int32_t extra_kcpu = 1000, packed_transaction::compression_type compression = packed_transaction::none ) {
   auto info = get_info();
   trx.expiration = info.head_block_time + tx_expiration;

   // Set tapos, default to last irreversible block if it's not specified by the user
   block_id_type ref_block_id = info.last_irreversible_block_id;
   try {
      fc::variant ref_block;
      if (!tx_ref_block_num_or_id.empty()) {
         ref_block = call(get_block_func, fc::mutable_variant_object("block_num_or_id", tx_ref_block_num_or_id));
         ref_block_id = ref_block["id"].as<block_id_type>();
      }
   } AAC_RETHROW_EXCEPTIONS(invalid_ref_block_exception, "Invalid reference block num or id: ${block_num_or_id}", ("block_num_or_id", tx_ref_block_num_or_id));
   trx.set_reference_block(ref_block_id);

   if (tx_force_unique) {
      trx.context_free_actions.emplace_back( generate_nonce_action() );
   }

   trx.max_cpu_usage_ms = tx_max_cpu_usage;
   trx.max_net_usage_words = (tx_max_net_usage + 7)/8;

   chain::signed_transaction strx(trx);
   if (!tx_skip_sign) {
      flat_set<public_key_type> required_keys;
      from_variant(determine_required_keys(trx), required_keys);
      strx = sign_transaction(trx, required_keys, info.chain_id);
   }

   return call(push_txn_func, packed_transaction(strx, compression));
}

fc::variant wallet_manager::push_actions(std::vector<chain::action>&& actions, int32_t extra_kcpu = 1000, packed_transaction::compression_type compression = packed_transaction::none ) {
   chain::signed_transaction trx;
   trx.actions = std::forward<decltype(actions)>(actions);

   return push_transaction(trx, extra_kcpu, compression);
}

chain::action create_newaccount(const name& creator, const name& newaccount, public_key_type owner, public_key_type active,
                    const permission_name& permission) {
   return chain::action {
      std::vector<chain::permission_level>{{creator, permission}},
      aacio::chain::newaccount{
         .creator      = creator,
         .name         = newaccount,
         .owner        = aacio::chain::authority{1, {{owner, 1}}, {}},
         .active       = aacio::chain::authority{1, {{active, 1}}, {}}
      }
   };
}

chain::action create_action(const std::vector<permission_level>& authorization, const account_name& code, const action_name& act, const fc::variant& args) {
   return chain::action{authorization, code, act, variant_to_bin(code, act, args)};
}

chain::action create_buyram(const name& creator, const name& newaccount, const asset& quantity,
                    const permission_name& permission) {
   fc::variant act_payload = fc::mutable_variant_object()
         ("payer", creator.to_string())
         ("receiver", newaccount.to_string())
         ("quant", quantity.to_string());
   return create_action(std::vector<chain::permission_level>{{creator, permission}},
                        config::system_account_name, N(buyram), act_payload);
}

chain::action create_buyrambytes(const name& creator, const name& newaccount, uint32_t numbytes,
                    const permission_name& permission) {
   fc::variant act_payload = fc::mutable_variant_object()
         ("payer", creator.to_string())
         ("receiver", newaccount.to_string())
         ("bytes", numbytes);
   return create_action(std::vector<chain::permission_level>{{creator, permission}},
                        config::system_account_name, N(buyrambytes), act_payload);
}

chain::action create_delegate(const name& from, const name& receiver, const asset& net, const asset& cpu, bool transfer,
                    const permission_name& permission) {
   fc::variant act_payload = fc::mutable_variant_object()
         ("from", from.to_string())
         ("receiver", receiver.to_string())
         ("stake_net_quantity", net.to_string())
         ("stake_cpu_quantity", cpu.to_string())
         ("transfer", transfer);
   return create_action(std::vector<chain::permission_level>{{from, permission}},
                        config::system_account_name, N(delegatebw), act_payload);
}

fc::variant wallet_manager::create_account(const ::create_account_params& p) {
   ::context = aacio::client::http::create_http_context();
   ::url = p.url;

   public_key_type owner_key, active_key;
   try {
      owner_key = public_key_type(p.owner);
   } AAC_RETHROW_EXCEPTIONS(public_key_type_exception, "Invalid owner public key: ${public_key}", ("public_key", p.owner));
   try {
      active_key = public_key_type(p.active);
   } AAC_RETHROW_EXCEPTIONS(public_key_type_exception, "Invalid active public key: ${public_key}", ("public_key", p.active));

   permission_name permission = p.creator == "aacio.cname" ? N(newacc) : config::active_name;

   action create = create_newaccount(p.creator, p.newaccount, owner_key, active_key, permission);
   action buyram = p.memory.empty() ? create_buyrambytes(p.creator, p.newaccount, p.memorykb * 1024, permission)
                    : create_buyram(p.creator, p.newaccount, asset::from_string(p.memory), permission);
   action delegate = create_delegate(p.creator, p.newaccount, asset::from_string(p.network), asset::from_string(p.cpu), false, permission);

   return push_actions( { create, buyram, delegate } );
}

fc::variant wallet_manager::easy_create_account(const std::string& url, const std::string& account, const std::string& key) {
   create_account_params p;
   p.url = url;
   p.creator = "aacio.cname";
   p.newaccount = account;
   p.owner = key;
   p.active = key;
   p.network = "0.0050 AAC";
   p.cpu = "0.0050 AAC";
   p.memory = "";
   p.memorykb = 4;

   return create_account(p);
}

fc::variant json_from_file_or_string(const std::string& file_or_str, fc::json::parse_type ptype = fc::json::legacy_parser)
{
   regex r("^[ \t]*[\{\[]");
   if ( !regex_search(file_or_str, r) && fc::is_regular_file(file_or_str) ) {
      return fc::json::from_file(file_or_str, ptype);
   } else {
      return fc::json::from_string(file_or_str, ptype);
   }
}

vector<chain::permission_level> get_account_permissions(const vector<std::string>& permissions) {
   auto fixedPermissions = permissions | boost::adaptors::transformed([](const std::string& p) {
      vector<std::string> pieces;
      split(pieces, p, boost::algorithm::is_any_of("@"));
      if( pieces.size() == 1 ) pieces.push_back( "active" );
      return chain::permission_level{ .actor = pieces[0], .permission = pieces[1] };
   });
   vector<chain::permission_level> accountPermissions;
   boost::range::copy(fixedPermissions, back_inserter(accountPermissions));
   return accountPermissions;
}

fc::variant wallet_manager::push_action(const ::push_action_params& p) {
   ::context = aacio::client::http::create_http_context();
   ::url = p.url;

   fc::variant action_args_var;
   if( !p.args.empty() ) {
      try {
         action_args_var = json_from_file_or_string(p.args, fc::json::relaxed_parser);
      } AAC_RETHROW_EXCEPTIONS(action_type_exception, "Fail to parse action JSON data='${data}'", ("data",p.args))
   }

   std::vector<std::string> tx_permission = { p.permission };
   auto accountPermissions = get_account_permissions(tx_permission);

   return push_actions({chain::action{accountPermissions, p.contract, p.action, variant_to_bin( p.contract, p.action, action_args_var ) }});
}


} // namespace wallet
} // namespace aacio
