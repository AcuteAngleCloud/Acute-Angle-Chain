/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */
#include <aacio/wallet_api_plugin/wallet_api_plugin.hpp>
#include <aacio/wallet_plugin/wallet_manager.hpp>
#include <aacio/chain/exceptions.hpp>
#include <aacio/chain/transaction.hpp>

#include <fc/variant.hpp>
#include <fc/io/json.hpp>

#include <chrono>

namespace aacio { namespace detail {
  struct wallet_api_plugin_empty {};
}}

FC_REFLECT(aacio::detail::wallet_api_plugin_empty, );

namespace aacio {

static appbase::abstract_plugin& _wallet_api_plugin = app().register_plugin<wallet_api_plugin>();

using namespace aacio;

#define CALL(api_name, api_handle, call_name, INVOKE, http_response_code) \
{std::string("/v1/" #api_name "/" #call_name), \
   [&api_handle](string, string body, url_response_callback cb) mutable { \
          try { \
             if (body.empty()) body = "{}"; \
             INVOKE \
             cb(http_response_code, fc::json::to_string(result)); \
          } catch (...) { \
             http_plugin::handle_exception(#api_name, #call_name, body, cb); \
          } \
       }}

#define INVOKE_R_R(api_handle, call_name, in_param) \
     auto result = api_handle.call_name(fc::json::from_string(body).as<in_param>());

#define INVOKE_R_R_R(api_handle, call_name, in_param0, in_param1) \
     const auto& vs = fc::json::json::from_string(body).as<fc::variants>(); \
     auto result = api_handle.call_name(vs.at(0).as<in_param0>(), vs.at(1).as<in_param1>());

#define INVOKE_R_R_R_R(api_handle, call_name, in_param0, in_param1, in_param2) \
     const auto& vs = fc::json::json::from_string(body).as<fc::variants>(); \
     auto result = api_handle.call_name(vs.at(0).as<in_param0>(), vs.at(1).as<in_param1>(), vs.at(2).as<in_param2>());

#define INVOKE_R_V(api_handle, call_name) \
     auto result = api_handle.call_name();

#define INVOKE_V_R(api_handle, call_name, in_param) \
     api_handle.call_name(fc::json::from_string(body).as<in_param>()); \
     aacio::detail::wallet_api_plugin_empty result;

#define INVOKE_V_R_R(api_handle, call_name, in_param0, in_param1) \
     const auto& vs = fc::json::json::from_string(body).as<fc::variants>(); \
     api_handle.call_name(vs.at(0).as<in_param0>(), vs.at(1).as<in_param1>()); \
     aacio::detail::wallet_api_plugin_empty result;

#define INVOKE_V_R_R_R(api_handle, call_name, in_param0, in_param1, in_param2) \
     const auto& vs = fc::json::json::from_string(body).as<fc::variants>(); \
     api_handle.call_name(vs.at(0).as<in_param0>(), vs.at(1).as<in_param1>(), vs.at(2).as<in_param2>()); \
     aacio::detail::wallet_api_plugin_empty result;

#define INVOKE_V_V(api_handle, call_name) \
     api_handle.call_name(); \
     aacio::detail::wallet_api_plugin_empty result;


void wallet_api_plugin::plugin_startup() {
   ilog("starting wallet_api_plugin");
   // lifetime of plugin is lifetime of application
   auto& wallet_mgr = app().get_plugin<wallet_plugin>().get_wallet_manager();

   app().get_plugin<http_plugin>().add_api({
       CALL(wallet, wallet_mgr, set_timeout,
            INVOKE_V_R(wallet_mgr, set_timeout, int64_t), 200),
       CALL(wallet, wallet_mgr, sign_transaction,
            INVOKE_R_R_R_R(wallet_mgr, sign_transaction, chain::signed_transaction, flat_set<public_key_type>, chain::chain_id_type), 201),
       CALL(wallet, wallet_mgr, sign_digest,
            INVOKE_R_R_R(wallet_mgr, sign_digest, chain::digest_type, public_key_type), 201),
       CALL(wallet, wallet_mgr, create,
            INVOKE_V_R_R(wallet_mgr, create, std::string, std::string), 201),
       CALL(wallet, wallet_mgr, open,
            INVOKE_V_R(wallet_mgr, open, std::string), 200),
       CALL(wallet, wallet_mgr, lock_all,
            INVOKE_V_V(wallet_mgr, lock_all), 200),
       CALL(wallet, wallet_mgr, lock,
            INVOKE_V_R(wallet_mgr, lock, std::string), 200),
       CALL(wallet, wallet_mgr, unlock,
            INVOKE_V_R_R(wallet_mgr, unlock, std::string, std::string), 200),
       CALL(wallet, wallet_mgr, import_key,
            INVOKE_V_R_R(wallet_mgr, import_key, std::string, std::string), 201),
       CALL(wallet, wallet_mgr, remove_key,
            INVOKE_V_R_R_R(wallet_mgr, remove_key, std::string, std::string, std::string), 201),
       CALL(wallet, wallet_mgr, create_key,
            INVOKE_R_R_R(wallet_mgr, create_key, std::string, std::string), 201),
       CALL(wallet, wallet_mgr, list_wallets,
            INVOKE_R_V(wallet_mgr, list_wallets), 200),
       CALL(wallet, wallet_mgr, list_keys,
            INVOKE_R_R_R(wallet_mgr, list_keys, std::string, std::string), 200),
       CALL(wallet, wallet_mgr, get_public_keys,
            INVOKE_R_V(wallet_mgr, get_public_keys), 200),
       CALL(wallet, wallet_mgr, change_password,
            INVOKE_V_R_R(wallet_mgr, change_password, std::string, std::string), 200),
       CALL(wallet, wallet_mgr, create_account,
            INVOKE_R_R(wallet_mgr, create_account, ::create_account_params), 202),
       CALL(wallet, wallet_mgr, easy_create_account,
            INVOKE_R_R_R_R(wallet_mgr, easy_create_account, std::string, std::string, std::string), 202),
       CALL(wallet, wallet_mgr, push_action,
            INVOKE_R_R(wallet_mgr, push_action, ::push_action_params), 202)
   });
}

void wallet_api_plugin::plugin_initialize(const variables_map& options) {
   try {
      const auto& _http_plugin = app().get_plugin<http_plugin>();
      if( !_http_plugin.is_on_loopback()) {
         if( !_http_plugin.is_secure()) {
            elog( "\n"
                  "********!!!SECURITY ERROR!!!********\n"
                  "*                                  *\n"
                  "* --       Wallet API           -- *\n"
                  "* - EXPOSED to the LOCAL NETWORK - *\n"
                  "* -  HTTP RPC is NOT encrypted   - *\n"
                  "* - Password and/or Private Keys - *\n"
                  "* - are at HIGH risk of exposure - *\n"
                  "*                                  *\n"
                  "************************************\n" );
         } else {
            wlog( "\n"
                  "**********SECURITY WARNING**********\n"
                  "*                                  *\n"
                  "* --       Wallet API           -- *\n"
                  "* - EXPOSED to the LOCAL NETWORK - *\n"
                  "* - Password and/or Private Keys - *\n"
                  "* -   are at risk of exposure    - *\n"
                  "*                                  *\n"
                  "************************************\n" );
         }
      }
   } FC_LOG_AND_RETHROW()
}


#undef INVOKE_R_R
#undef INVOKE_R_R_R_R
#undef INVOKE_R_V
#undef INVOKE_V_R
#undef INVOKE_V_R_R
#undef INVOKE_V_V
#undef CALL

}
