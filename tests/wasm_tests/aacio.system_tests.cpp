#include <boost/test/unit_test.hpp>
#include <aacio/testing/tester.hpp>
#include <aacio/chain/contracts/abi_serializer.hpp>
#include <aacio/chain_plugin/chain_plugin.hpp>

#include <aacio.system/aacio.system.wast.hpp>
#include <aacio.system/aacio.system.abi.hpp>

#include <Runtime/Runtime.h>

#include <fc/variant_object.hpp>

#ifdef NON_VALIDATING_TEST
#define TESTER tester
#else
#define TESTER validating_tester
#endif

using namespace aacio::testing;
using namespace aacio;
using namespace aacio::chain;
using namespace aacio::chain::contracts;
using namespace aacio::chain_apis;
using namespace aacio::testing;
using namespace fc;

using mvo = fc::mutable_variant_object;

class aacio_system_tester : public TESTER {
public:

   aacio_system_tester() {
      produce_blocks( 2 );

      create_accounts( { N(alice), N(bob), N(carol) } );
      produce_blocks( 1000 );

      set_code( config::system_account_name, aacio_system_wast );
      set_abi( config::system_account_name, aacio_system_abi );

      produce_blocks();

      const auto& accnt = control->get_database().get<account_object,by_name>( config::system_account_name );
      abi_def abi;
      BOOST_REQUIRE_EQUAL(abi_serializer::to_abi(accnt.abi, abi), true);
      abi_ser.set_abi(abi);
      /*
      const global_property_object &gpo = control->get_global_properties();
      FC_ASSERT(0 < gpo.active_producers.producers.size(), "No producers");
      producer_name = (string)gpo.active_producers.producers.front().producer_name;
      */
   }

   action_result push_action( const account_name& signer, const action_name &name, const variant_object &data, bool auth = true ) {
         string action_type_name = abi_ser.get_action_type(name);

         action act;
         act.account = config::system_account_name;
         act.name = name;
         act.data = abi_ser.variant_to_binary( action_type_name, data );

         return base_tester::push_action( std::move(act), auth ? uint64_t(signer) : signer == N(bob) ? N(alice) : N(bob) );
   }

   action_result stake( const account_name& from, const account_name& to, const string& net, const string& cpu, const string& storage ) {
      return push_action( name(from), N(delegatebw), mvo()
                          ("from",     from)
                          ("receiver", to)
                          ("stake_net", net)
                          ("stake_cpu", cpu)
                          ("stake_storage", storage)
      );
   }

   action_result stake( const account_name& acnt, const string& net, const string& cpu, const string& storage ) {
      return stake( acnt, acnt, net, cpu, storage );
   }

   action_result unstake( const account_name& from, const account_name& to, const string& net, const string& cpu, uint64_t bytes ) {
      return push_action( name(from), N(undelegatebw), mvo()
                          ("from",     from)
                          ("receiver", to)
                          ("unstake_net", net)
                          ("unstake_cpu", cpu)
                          ("unstake_bytes", bytes)
      );
   }

   action_result unstake( const account_name& acnt, const string& net, const string& cpu, uint64_t bytes ) {
      return unstake( acnt, acnt, net, cpu, bytes );
   }

   static fc::variant_object producer_parameters_example( int n ) {
      return mutable_variant_object()
         ("base_per_transaction_net_usage", 100 + n)
         ("base_per_transaction_cpu_usage", 100 + n)
         ("base_per_action_cpu_usage", 100 + n)
         ("base_setcode_cpu_usage", 100 + n)
         ("per_signature_cpu_usage", 100 + n)
         ("per_lock_net_usage", 100 + n )
         ("context_free_discount_cpu_usage_num", 1 + n )
         ("context_free_discount_cpu_usage_den", 100 + n )
         ("max_transaction_cpu_usage", 1000000 + n )
         ("max_transaction_net_usage", 1000000 + n )
         ("max_block_cpu_usage", 10000000 + n )
         ("target_block_cpu_usage_pct", 10 + n )
         ("max_block_net_usage", 10000000 + n )
         ("target_block_net_usage_pct", 10 + n )
         ("max_transaction_lifetime", 3600 + n)
         ("max_transaction_exec_time", 9900 + n)
         ("max_authority_depth", 6 + n)
         ("max_inline_depth", 4 + n)
         ("max_inline_action_size", 4096 + n)
         ("max_generated_transaction_count", 10 + n)
         ("percent_of_max_inflation_rate", 50 + n)
         ("storage_reserve_ratio", 100 + n);
   }

   action_result regproducer( const account_name& acnt, int params_fixture = 1 ) {
      return push_action( acnt, N(regproducer), mvo()
                          ("producer",  name(acnt).to_string() )
                          ("producer_key", fc::raw::pack( get_public_key( acnt, "active" ) ) )
                          ("prefs", producer_parameters_example( params_fixture ) )
      );
   }

   uint32_t last_block_time() const {
      return time_point_sec( control->head_block_time() ).sec_since_epoch();
   }

   asset get_balance( const account_name& act ) {
      //return get_currency_balance( config::system_account_name, symbol(SY(4,AAC)), act );
      //temporary code. current get_currency_balancy uses table name N(accounts) from currency.h
      //generic_currency table name is N(account).
      const auto& db  = control->get_database();
      const auto* tbl = db.find<contracts::table_id_object, contracts::by_code_scope_table>(boost::make_tuple(config::system_account_name, act, N(account)));
      share_type result = 0;

      // the balance is implied to be 0 if either the table or row does not exist
      if (tbl) {
         const auto *obj = db.find<contracts::key_value_object, contracts::by_scope_primary>(boost::make_tuple(tbl->id, symbol(SY(4,AAC)).value()));
         if (obj) {
            //balance is the second field after symbol, so skip the symbol
            fc::datastream<const char *> ds(obj->value.data(), obj->value.size());
            fc::raw::unpack(ds, result);
         }
      }
      return asset( result, symbol(SY(4,AAC)) );
   }

   fc::variant get_total_stake( const account_name& act ) {
      vector<char> data = get_row_by_account( config::system_account_name, act, N(totalband), act );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "total_resources", data );
   }

   fc::variant get_voter_info( const account_name& act ) {
      vector<char> data = get_row_by_account( config::system_account_name, config::system_account_name, N(voters), act );
      return data.empty() ? fc::variant() : abi_ser.binary_to_variant( "voter_info", data );
   }
  
   fc::variant get_producer_info( const account_name& act ) {
      vector<char> data = get_row_by_account( config::system_account_name, config::system_account_name, N(producerinfo), act );
      return abi_ser.binary_to_variant( "producer_info", data );
   }

   abi_serializer abi_ser;
};

fc::mutable_variant_object voter( account_name acct ) {
   return mutable_variant_object()
      ("owner", acct)
      ("proxy", name(0).to_string())
      ("is_proxy", 0)
      ("staked", 0)
      ("unstaking", 0)
      ("unstake_per_week", 0)
      ("proxied_votes", 0)
      ("producers", variants() )
      ("deferred_trx_id", 0)
      ("last_unstake", 0);
}

fc::mutable_variant_object voter( account_name acct, const string& vote_stake ) {
   return voter( acct )( "staked", asset::from_string( vote_stake ).amount );
}

fc::mutable_variant_object proxy( account_name acct ) {
   return voter( acct )( "is_proxy", 1 );
}

inline uint64_t M( const string& aac_str ) {
   return asset::from_string( aac_str ).amount;
}

BOOST_AUTO_TEST_SUITE(aacio_system_tests)

BOOST_FIXTURE_TEST_CASE( stake_unstake, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC", config::system_account_name );
   BOOST_REQUIRE_EQUAL( asset::from_string("1000.0000 AAC"), get_balance( "alice" ) );

   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "200.0000 AAC", "100.0000 AAC", "500.0000 AAC" ) );

   auto total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("200.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("100.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("500.0000 AAC").amount, total["storage_stake"].as_uint64());
   REQUIRE_MATCHING_OBJECT( voter( "alice", "300.0000 AAC"), get_voter_info( "alice" ) );

   auto bytes = total["storage_bytes"].as_uint64();
   BOOST_REQUIRE_EQUAL( true, 0 < bytes );

   BOOST_REQUIRE_EQUAL( asset::from_string("200.0000 AAC"), get_balance( "alice" ) );

   //unstake
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice", "200.0000 AAC", "100.0000 AAC", bytes ) );
   total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("0.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("0.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("0.0000 AAC").amount, total["storage_stake"].as_uint64());
   BOOST_REQUIRE_EQUAL( 0, total["storage_bytes"].as_uint64());
   REQUIRE_MATCHING_OBJECT( voter( "alice", "0.00 AAC" ), get_voter_info( "alice" ) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( asset::from_string("200.0000 AAC"), get_balance( "alice" ) );

   //after 2 days balance should not be available yet
   produce_block( fc::hours(3*24-1) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( asset::from_string("200.0000 AAC"), get_balance( "alice" ) );
   //after 3 days funds should be released
   produce_block( fc::hours(1) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( asset::from_string("1000.0000 AAC"), get_balance( "alice" ) );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( fail_without_auth, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );

   BOOST_REQUIRE_EQUAL( error("missing authority of alice"),
                        push_action( N(alice), N(delegatebw), mvo()
                                    ("from",     "alice")
                                    ("receiver", "bob")
                                    ("stake_net", "10.0000 AAC")
                                    ("stake_cpu", "10.0000 AAC")
                                    ("stake_storage", "10.0000 AAC"),
                                    false
                        )
   );

   BOOST_REQUIRE_EQUAL( error("missing authority of alice"),
                        push_action(N(alice), N(undelegatebw), mvo()
                                    ("from",     "alice")
                                    ("receiver", "bob")
                                    ("unstake_net", "200.0000 AAC")
                                    ("unstake_cpu", "100.0000 AAC")
                                    ("unstake_bytes", 0)
                                    ,false
                        )
   );
   //REQUIRE_MATCHING_OBJECT( , get_voter_info( "alice" ) );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( stake_negative, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );

   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: must stake a positive amount"),
                        stake( "alice", "-0.0001 AAC", "0.0000 AAC", "0.0000 AAC" )
   );

   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: must stake a positive amount"),
                        stake( "alice", "0.0000 AAC", "-0.0001 AAC", "0.0000 AAC" )
   );

   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: must stake a positive amount"),
                        stake( "alice", "00.0000 AAC", "00.0000 AAC", "0.0000 AAC" )
   );

   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: must stake a positive amount"),
                        stake( "alice", "0.0000 AAC", "00.0000 AAC", "0.0000 AAC" )
   );

   BOOST_REQUIRE_EQUAL( true, get_voter_info( "alice" ).is_null() );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( unstake_negative, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );

   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "bob", "200.0001 AAC", "100.0001 AAC", "300.0000 AAC" ) );

   auto total = get_total_stake( "bob" );
   BOOST_REQUIRE_EQUAL( asset::from_string("200.0001 AAC").amount, total["net_weight"].as_uint64());
   REQUIRE_MATCHING_OBJECT( voter( "alice", "300.0002 AAC" ), get_voter_info( "alice" ) );

   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: must unstake a positive amount"),
                        unstake( "alice", "bob", "-1.0000 AAC", "0.0000 AAC", 0 )
   );

   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: must unstake a positive amount"),
                        unstake( "alice", "bob", "0.0000 AAC", "-1.0000 AAC", 0 )
   );

   //unstake all zeros
   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: must unstake a positive amount"),
                        unstake( "alice", "bob", "0.0000 AAC", "0.0000 AAC", 0 )
   );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( unstake_more_than_at_stake, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "200.0000 AAC", "100.0000 AAC", "150.0000 AAC" ) );

   auto total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("200.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("100.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("150.0000 AAC").amount, total["storage_stake"].as_uint64());
   auto bytes = total["storage_bytes"].as_uint64();
   BOOST_REQUIRE_EQUAL( true, 0 < bytes );

   BOOST_REQUIRE_EQUAL( asset::from_string("550.0000 AAC"), get_balance( "alice" ) );

   //trying to unstake more net bandwith than at stake
   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: insufficient staked net bandwidth"),
                        unstake( "alice", "200.0001 AAC", "0.0000 AAC", 0 )
   );

   //trying to unstake more cpu bandwith than at stake
   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: insufficient staked cpu bandwidth"),
                        unstake( "alice", "000.0000 AAC", "100.0001 AAC", 0 )
   );

   //trying to unstake more storage than at stake
   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: insufficient staked storage"),
                        unstake( "alice", "000.0000 AAC", "0.0000 AAC", bytes+1 )
   );

   //check that nothing has changed
   total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("200.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("100.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("150.0000 AAC").amount, total["storage_stake"].as_uint64());
   BOOST_REQUIRE_EQUAL( bytes, total["storage_bytes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( asset::from_string("550.0000 AAC"), get_balance( "alice" ) );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( delegate_to_another_user, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );

   BOOST_REQUIRE_EQUAL( success(), stake ( "alice", "bob", "200.0000 AAC", "100.0000 AAC", "80.0000 AAC" ) );

   auto total = get_total_stake( "bob" );
   BOOST_REQUIRE_EQUAL( asset::from_string("200.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("100.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("80.0000 AAC").amount, total["storage_stake"].as_uint64());
   auto bytes = total["storage_bytes"].as_uint64();
   BOOST_REQUIRE_EQUAL( true, 0 < bytes );
   BOOST_REQUIRE_EQUAL( asset::from_string("620.0000 AAC"), get_balance( "alice" ) );
   //all voting power goes to alice
   REQUIRE_MATCHING_OBJECT( voter( "alice", "300.0000 AAC" ), get_voter_info( "alice" ) );
   //but not to bob
   BOOST_REQUIRE_EQUAL( true, get_voter_info( "bob" ).is_null() );

   //bob should not be able to unstake what was staked by alice
   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: unable to find key"),
                        unstake( "bob", "0.0000 AAC", "10.0000 AAC", bytes )
   );

   issue( "carol", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "carol", "bob", "20.0000 AAC", "10.0000 AAC", "8.0000 AAC" ) );
   total = get_total_stake( "bob" );
   BOOST_REQUIRE_EQUAL( asset::from_string("220.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("110.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("88.0000 AAC").amount, total["storage_stake"].as_uint64());
   auto bytes2 = total["storage_bytes"].as_uint64();
   BOOST_REQUIRE_EQUAL( true, bytes < bytes2 );
   BOOST_REQUIRE_EQUAL( asset::from_string("962.0000 AAC"), get_balance( "carol" ) );
   REQUIRE_MATCHING_OBJECT( voter( "carol", "30.0000 AAC" ), get_voter_info( "carol" ) );

   //alice should not be able to unstake money staked by carol
   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: insufficient staked net bandwidth"),
                        unstake( "alice", "bob", "2001.0000 AAC", "1.0000 AAC", bytes-1 )
   );

   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: insufficient staked cpu bandwidth"),
                        unstake( "alice", "bob", "1.0000 AAC", "101.0000 AAC", bytes-1 )
   );

    BOOST_REQUIRE_EQUAL( error("condition: assertion failed: insufficient staked storage"),
                         unstake( "alice", "bob", "1.0000 AAC", "1.0000 AAC", bytes+1 )
    );

   total = get_total_stake( "bob" );
   BOOST_REQUIRE_EQUAL( asset::from_string("220.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("110.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("88.0000 AAC").amount, total["storage_stake"].as_uint64());
   BOOST_REQUIRE_EQUAL( bytes2, total["storage_bytes"].as_uint64());
   //balance should not change after unsuccessfull attempts to unstake
   BOOST_REQUIRE_EQUAL( asset::from_string("620.0000 AAC"), get_balance( "alice" ) );
   //voting power too
   REQUIRE_MATCHING_OBJECT( voter( "alice", "300.0000 AAC" ), get_voter_info( "alice" ) );
   REQUIRE_MATCHING_OBJECT( voter( "carol", "30.0000 AAC" ), get_voter_info( "carol" ) );
   BOOST_REQUIRE_EQUAL( true, get_voter_info( "bob" ).is_null() );
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( stake_unstake_separate, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( asset::from_string("1000.0000 AAC"), get_balance( "alice" ) );

   //everything at once
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "10.0000 AAC", "20.0000 AAC", "30.0000 AAC" ) );
   auto total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("10.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("20.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("30.0000 AAC").amount, total["storage_stake"].as_uint64());
   auto bytes = total["storage_bytes"].as_uint64();
   BOOST_REQUIRE_EQUAL( true, 0 < bytes );

   //cpu
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "100.0000 AAC", "0.0000 AAC", "0.0000 AAC" ) );
   total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("110.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("20.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("30.0000 AAC").amount, total["storage_stake"].as_uint64());
   BOOST_REQUIRE_EQUAL( bytes, total["storage_bytes"].as_uint64() );

   //net
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "0.0000 AAC", "200.0000 AAC", "0.0000 AAC" ) );
   total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("110.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("220.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("30.0000 AAC").amount, total["storage_stake"].as_uint64());
   BOOST_REQUIRE_EQUAL( bytes, total["storage_bytes"].as_uint64() );

   //storage
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "0.0000 AAC", "0.0000 AAC", "300.0000 AAC" ) );
   total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("110.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("220.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("330.0000 AAC").amount, total["storage_stake"].as_uint64());
   auto bytes2 = total["storage_bytes"].as_uint64();
   BOOST_REQUIRE_EQUAL( true, bytes < bytes2 );

   //unstake cpu
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice", "100.0000 AAC", "0.0000 AAC", 0 ) );
   total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("10.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("220.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("330.0000 AAC").amount, total["storage_stake"].as_uint64());
   BOOST_REQUIRE_EQUAL( bytes2, total["storage_bytes"].as_uint64());

   //unstake net
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice", "0.0000 AAC", "200.0000 AAC", 0 ) );
   total = get_total_stake( "alice" );
   BOOST_REQUIRE_EQUAL( asset::from_string("10.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("20.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("330.0000 AAC").amount, total["storage_stake"].as_uint64());
   BOOST_REQUIRE_EQUAL( bytes2, total["storage_bytes"].as_uint64());

   //unstake cpu
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice", "0.0000 AAC", "0.0000 AAC", bytes2 / 2 ) );
   total = get_total_stake( "alice" );
   auto storage_left = M("330.0000 AAC") - M("330.0000 AAC") * (bytes2 / 2) / bytes2;
   BOOST_REQUIRE_EQUAL( asset::from_string("10.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("20.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( storage_left, total["storage_stake"].as_uint64());
   BOOST_REQUIRE_EQUAL( bytes2 - bytes2/2, total["storage_bytes"].as_uint64());
} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( adding_stake_partial_unstake, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "bob", "200.0000 AAC", "100.0000 AAC", "80.0000 AAC" ) );

   auto total = get_total_stake( "bob" );
   auto bytes0 = total["storage_bytes"].as_uint64();
   REQUIRE_MATCHING_OBJECT( voter( "alice", "300.0000 AAC" ), get_voter_info( "alice" ) );

   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "bob", "100.0000 AAC", "50.0000 AAC", "40.0000 AAC" ) );

   total = get_total_stake( "bob" );
   BOOST_REQUIRE_EQUAL( asset::from_string("300.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("150.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("120.0000 AAC").amount, total["storage_stake"].as_uint64());
   auto bytes = total["storage_bytes"].as_uint64();
   BOOST_REQUIRE_EQUAL( true, bytes0 < bytes );
   REQUIRE_MATCHING_OBJECT( voter( "alice", "450.0000 AAC" ), get_voter_info( "alice" ) );
   BOOST_REQUIRE_EQUAL( asset::from_string("430.0000 AAC"), get_balance( "alice" ) );

   //unstake a share
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice", "bob", "150.0000 AAC", "75.0000 AAC", bytes / 2 ) );

   total = get_total_stake( "bob" );
   BOOST_REQUIRE_EQUAL( asset::from_string("150.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("75.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( bytes-bytes/2, total["storage_bytes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( asset::from_string("120.0000 AAC").amount - asset::from_string("120.0000 AAC").amount * (bytes/2)/bytes,
                        total["storage_stake"].as_uint64());
   REQUIRE_MATCHING_OBJECT( voter( "alice", "225.0000 AAC" ), get_voter_info( "alice" ) );

   //unstake more
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice", "bob", "50.0000 AAC", "25.0000 AAC", 0 ) );
   total = get_total_stake( "bob" );
   BOOST_REQUIRE_EQUAL( asset::from_string("100.0000 AAC").amount, total["net_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( asset::from_string("50.0000 AAC").amount, total["cpu_weight"].as_uint64());
   BOOST_REQUIRE_EQUAL( bytes-bytes/2, total["storage_bytes"].as_uint64() );
   REQUIRE_MATCHING_OBJECT( voter( "alice", "150.0000 AAC" ), get_voter_info( "alice" ) );

   //combined amount should be available only in 3 days
   produce_block( fc::days(2) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( asset::from_string("430.0000 AAC"), get_balance( "alice" ) );
   produce_block( fc::days(1) );
   produce_blocks(1);
   BOOST_REQUIRE_EQUAL( asset::from_string("790.0000 AAC"), get_balance( "alice" ) );

} FC_LOG_AND_RETHROW()

// Tests for voting

BOOST_FIXTURE_TEST_CASE( producer_register_unregister, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );

   fc::variant params = producer_parameters_example(1);
   vector<char> key = fc::raw::pack( fc::crypto::public_key( std::string("AAC84snobCGtpQvTTfVaxvMuxGDd4p2LhtQhbWX1yWAU5yt4tiB55") ) );
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(regproducer), mvo()
                                               ("producer",  "alice")
                                               ("producer_key", key )
                                               ("prefs", params)
                        )
   );

   auto info = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( "alice", info["owner"].as_string() );
   BOOST_REQUIRE_EQUAL( 0, info["total_votes"].as_uint64() );
   REQUIRE_MATCHING_OBJECT( params, info["prefs"] );
   BOOST_REQUIRE_EQUAL( string(key.begin(), key.end()), to_string(info["packed_key"]) );

   //abc
   vector<char> pba = fc::raw::pack(fc::crypto::public_key(std::string("AAC7fhfs1j5BBQC9bsM5c8c2WZy6NPG6mbAGMuuYJCnKNHfb3vrND")));
   ilog("pb.a,${pba}", ("pba", pba));
   std::cout << fc::json::to_pretty_string(pba) << std::endl;

   vector<char> pbb = fc::raw::pack(fc::crypto::public_key(std::string("AAC5nqrTB9Tj4B41AUrHPUf7sfnN1r25b6v6qtYM57n74bpqwXd3e")));
   ilog("pb.b,${pbb}", ("pbb", pbb));
   std::cout << fc::json::to_pretty_string(pbb) << std::endl;

   vector<char> pbc = fc::raw::pack(fc::crypto::public_key(std::string("AAC89NSQo1nm3Re39nSujnTgXtNjS1X5Mgdzn92Ni5zCt1Gwnvfqt")));
   ilog("pb.c,${pbc}", ("pbc", pbc));
   std::cout << fc::json::to_pretty_string(pbc) << std::endl;

   vector<char> pbd = fc::raw::pack(fc::crypto::public_key(std::string("AAC6a5pr4DS4CksCQSHqTdKMPbAdCyrE4b7QExDwTuCxH1vbkYMqG")));
   ilog("pb.d,${pbd}", ("pbd", pbd));
   std::cout << fc::json::to_pretty_string(pbd) << std::endl;

   vector<char> pbe = fc::raw::pack(fc::crypto::public_key(std::string("AAC5vyM6odJjC4GvJ1zggnd2sxG6p9GC64adK7EcWmkXczQTEHohm")));
   ilog("pb.e,${pbe}", ("pbe", pbe));
   std::cout << fc::json::to_pretty_string(pbe) << std::endl;

   vector<char> pbf = fc::raw::pack(fc::crypto::public_key(std::string("AAC7k7nW1iEhnKqmdZV7oiRuPPsF1wnnFjh2VRSQpQxn5RDfdv5Va")));
   ilog("pb.f,${pbf}", ("pbf", pbf));
   std::cout << fc::json::to_pretty_string(pbf) << std::endl;

   vector<char> pbg = fc::raw::pack(fc::crypto::public_key(std::string("AAC8QhRVRxVzuyuJEy5gdTHN6dDkBQhTBpXt5G5DEip7ByVS1RjfX")));
   ilog("pb.g,${pbg}", ("pbg", pbg));
   std::cout << fc::json::to_pretty_string(pbg) << std::endl;

   vector<char> pbh = fc::raw::pack(fc::crypto::public_key(std::string("AAC7vrwPVPbfFni337mWSLSqDkz4mrDMFLryN2X7vXwbK4iJLRvzn")));
   ilog("pb.h,${pbh}", ("pbh", pbh));
   std::cout << fc::json::to_pretty_string(pbh) << std::endl;

   vector<char> pbi = fc::raw::pack(fc::crypto::public_key(std::string("AAC5Uf6epwYFN5z9yBufMhwirgDXPWj7Lqo89CwJbuchxWCQHSpcp")));
   ilog("pb.i,${pbi}", ("pbi", pbi));
   std::cout << fc::json::to_pretty_string(pbi) << std::endl;

   vector<char> pbj = fc::raw::pack(fc::crypto::public_key(std::string("AAC6zvtWmDHSdpf9xSpJh98tksULrMpWuyxQnrBU2r5eg2UnccMr8")));
   ilog("pb.j,${pbj}", ("pbj", pbj));
   std::cout << fc::json::to_pretty_string(pbj) << std::endl;

   vector<char> pbk = fc::raw::pack(fc::crypto::public_key(std::string("AAC6guPa23KpMytvivSQbcfjwqzQaZ7fDc6NGjoRj3ffiiG4yiWfR")));
   ilog("pb.k,${pbk}", ("pbk", pbk));
   std::cout << fc::json::to_pretty_string(pbk) << std::endl;

   vector<char> pbl = fc::raw::pack(fc::crypto::public_key(std::string("AAC7ChpmNdBsStzbNxCaM267PYCnEeFyELfDeyR5N98zrdbx8iAqw")));
   ilog("pb.l,${pbl}", ("pbl", pbl));
   std::cout << fc::json::to_pretty_string(pbl) << std::endl;

   vector<char> pbm = fc::raw::pack(fc::crypto::public_key(std::string("AAC61ThiwJCyuYFYTjhjXtd5CVy1ohD5eLfrhRJcFQYm5ke7UpmTB")));
   ilog("pb.m,${pbm}", ("pbm", pbm));
   std::cout << fc::json::to_pretty_string(pbm) << std::endl;

   vector<char> pbn = fc::raw::pack(fc::crypto::public_key(std::string("AAC6XoHTgazmwpVTuRV5qzYdJPrMYRKAMzHuL3CkxB6PwhGMBaRwp")));
   ilog("pb.n,${pbn}", ("pbn", pbn));
   std::cout << fc::json::to_pretty_string(pbn) << std::endl;

   vector<char> pbo = fc::raw::pack(fc::crypto::public_key(std::string("AAC5CPy6gCWZRaUVP9Fy1UCzzYFXvoVZNLWzUY1mnN1UFVd8uoq57")));
   ilog("pb.o,${pbo}", ("pbo", pbo));
   std::cout << fc::json::to_pretty_string(pbo) << std::endl;

   vector<char> pbp = fc::raw::pack(fc::crypto::public_key(std::string("AAC7KvyP1SZmCYfJg3ACALFaHP21RRdaQLwPf82YZncBDoh2LmW92")));
   ilog("pb.p,${pbp}", ("pbp", pbp));
   std::cout << fc::json::to_pretty_string(pbp) << std::endl;

   vector<char> pbq = fc::raw::pack(fc::crypto::public_key(std::string("AAC5njWHi3idoXxrNKjanQpQAEjJxbH7Vm5qVhqdPJhJZwcuNuYAf")));
   ilog("pb.q,${pbq}", ("pbq", pbq));
   std::cout << fc::json::to_pretty_string(pbq) << std::endl;

   vector<char> pbr = fc::raw::pack(fc::crypto::public_key(std::string("AAC5tWUbDE9oVKSc2nRRb5C8CfPuJJSN4sHnyJCgVoVC8PaBbzRZV")));
   ilog("pb.r,${pbr}", ("pbr", pbr));
   std::cout << fc::json::to_pretty_string(pbr) << std::endl;

   vector<char> pbs = fc::raw::pack(fc::crypto::public_key(std::string("AAC7NTEoFWFj4XHbiVyfuKEhGDQdokyaStWCj5czPK9qWeznSBKd9")));
   ilog("pb.s,${pbs}", ("pbs", pbs));
   std::cout << fc::json::to_pretty_string(pbs) << std::endl;

   vector<char> pbt = fc::raw::pack(fc::crypto::public_key(std::string("AAC5H6tAXckc3mdXsBXStXtmUB8uZfJ1SBU68dMfCpzZtaJtx9gAA")));
   ilog("pb.t,${pbt}", ("pbt", pbt));
   std::cout << fc::json::to_pretty_string(pbt) << std::endl;

   vector<char> pbu = fc::raw::pack(fc::crypto::public_key(std::string("AAC6qAEZPYTMdChgvgYd2HBhCBki48ibCCx5rpfm3RmJGP2yxn9hg")));
   ilog("pb.u,${pbu}", ("pbu", pbu));
   std::cout << fc::json::to_pretty_string(pbu) << std::endl;

   vector<char> pbv = fc::raw::pack(fc::crypto::public_key(std::string("AAC54k1zsVNr8KMci1zHXjJTv57trJbpusoVvnYSF5Jz1CNVGR9TC")));
   ilog("pb.v,${pbv}", ("pbv", pbv));
   std::cout << fc::json::to_pretty_string(pbv) << std::endl;

   vector<char> pbw = fc::raw::pack(fc::crypto::public_key(std::string("AAC8L2dk8wQ8BjzD8f61BbkgbmvAzMYknXDDTTccQsVBoD8GNuN9E")));
   ilog("pb.w,${pbw}", ("pbw", pbw));
   std::cout << fc::json::to_pretty_string(pbw) << std::endl;

   vector<char> pbx = fc::raw::pack(fc::crypto::public_key(std::string("AAC87rqjEHuajhn9X1ihfJAom7z33p69ZqDSZzPgVTVLA2LPaq73b")));
   ilog("pb.x,${pbx}", ("pbx", pbx));
   std::cout << fc::json::to_pretty_string(pbx) << std::endl;

   vector<char> pby = fc::raw::pack(fc::crypto::public_key(std::string("AAC5E8QsUVbjhG3NdH1LedzpYbXr5HNkQUTKWcW1Fkxi76Xoaxpzj")));
   ilog("pb.y,${pby}", ("pby", pby));
   std::cout << fc::json::to_pretty_string(pby) << std::endl;

   vector<char> pbz = fc::raw::pack(fc::crypto::public_key(std::string("AAC7Lk8VzMN1UP3GUNP9QNDm2fjjhHjtrnVtrSeMNAimSuvjU5Nur")));
   ilog("pb.z,${pbz}", ("pbz", pbz));
   std::cout << fc::json::to_pretty_string(pbz) << std::endl;

   //abc end

/*
   //abc
   vector<char> likey = fc::raw::pack(fc::crypto::public_key(std::string("AAC5gV51Npp5nKdmMa2hJgfAQTwUyrridZMauveZUyDBBSFpQDtZM")));
   ilog("------------li active key-----------${likey}", ("likey", likey));
   std::cout << fc::json::to_pretty_string(likey) << std::endl;
   //abc end

   //abc
   vector<char> shangkey = fc::raw::pack(fc::crypto::public_key(std::string("AAC5ar66dCYpQC715kZhmYf69YCq2Ht1X8AY4Zp31nfvnqZZRLmoK")));
   ilog("------------shang active key-----------${shangkey}", ("shangkey", shangkey));
   std::cout << fc::json::to_pretty_string(shangkey) << std::endl;
   //abc end

   //abc
   vector<char> zhongkey = fc::raw::pack(fc::crypto::public_key(std::string("AAC7sCjRdmL7c5khiLwwstU7LKPY3wGWx8XFfwviGjaobzzU2vP9q")));
   ilog("------------zhong active key-----------${zhongkey}", ("zhongkey", zhongkey));
   std::cout << fc::json::to_pretty_string(zhongkey) << std::endl;
   //abc end

   //abc
   vector<char> kevinkey = fc::raw::pack(fc::crypto::public_key(std::string("AAC5Wd9zeYJKCSxsmwaQvpkjs8PDeHozjzkZpqTxvCPPeckfFV6YG")));
   ilog("------------kevin active key-----------${kevinkey}", ("kevinkey", kevinkey));
   std::cout << fc::json::to_pretty_string(kevinkey) << std::endl;
   //abc end
*/

   //call regproducer again to change parameters
   fc::variant params2 = producer_parameters_example(2);

   vector<char> key2 = fc::raw::pack( fc::crypto::public_key( std::string("AACR16EPHFSKVYHBjQgxVGQPrwCxTg7BbZ69H9i4gztN9deKTEXYne4") ) );

   
   //abc
   auto abc = mvo()("producer", "alice")("producer_key", key2)("prefs", params2);
   ilog("-----------------------------${abc}", ("abc", abc));
   std::cout << fc::json::to_pretty_string(abc) << std::endl;
   //abc end

   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(regproducer), mvo()
                                               ("producer",  "alice")
                                               ("producer_key", key2 )
                                               ("prefs", params2)
                        )
   );

   info = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( "alice", info["owner"].as_string() );
   BOOST_REQUIRE_EQUAL( 0, info["total_votes"].as_uint64() );
   REQUIRE_MATCHING_OBJECT( params2, info["prefs"] );
   BOOST_REQUIRE_EQUAL( string(key2.begin(), key2.end()), to_string(info["packed_key"]) );

   //unregister producer
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(unregprod), mvo()
                                               ("producer",  "alice")
                        )
   );
   info = get_producer_info( "alice" );
   //key should be empty
   BOOST_REQUIRE_EQUAL( true, to_string(info["packed_key"]).empty() );
   //everything else should stay the same
   BOOST_REQUIRE_EQUAL( "alice", info["owner"].as_string() );
   BOOST_REQUIRE_EQUAL( 0, info["total_votes"].as_uint64() );
   REQUIRE_MATCHING_OBJECT( params2, info["prefs"] );

   //unregister bob who is not a producer
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: producer not found" ),
                        push_action( N(bob), N(unregprod), mvo()
                                     ("producer",  "bob")
                        )
   );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( vote_for_producer, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   fc::variant params = producer_parameters_example(1);
   vector<char> key = fc::raw::pack( get_public_key( N(alice), "active" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproducer), mvo()
                                               ("producer",  "alice")
                                               ("producer_key", key )
                                               ("prefs", params)
                        )
   );
   auto prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( "alice", prod["owner"].as_string() );
   BOOST_REQUIRE_EQUAL( 0, prod["total_votes"].as_uint64() );
   REQUIRE_MATCHING_OBJECT( params, prod["prefs"]);
   BOOST_REQUIRE_EQUAL( string(key.begin(), key.end()), to_string(prod["packed_key"]) );

   issue( "bob", "2000.0000 AAC",  config::system_account_name );
   issue( "carol", "3000.0000 AAC",  config::system_account_name );

   //bob makes stake
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "11.0000 AAC", "0.1111 AAC", "0.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( asset::from_string("1988.8889 AAC"), get_balance( "bob" ) );
   REQUIRE_MATCHING_OBJECT( voter( "bob", "11.1111 AAC" ), get_voter_info( "bob" ) );

   //bob votes for alice
   BOOST_REQUIRE_EQUAL( success(), push_action(N(bob), N(voteproducer), mvo()
                                               ("voter",  "bob")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(alice) } )
                        )
   );

   //check that producer parameters stay the same after voting
   prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 111111, prod["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( "alice", prod["owner"].as_string() );
   REQUIRE_MATCHING_OBJECT( params, prod["prefs"]);
   BOOST_REQUIRE_EQUAL( string(key.begin(), key.end()), to_string(prod["packed_key"]) );

   //carol makes stake
   BOOST_REQUIRE_EQUAL( success(), stake( "carol", "22.0000 AAC", "0.2222 AAC", "0.0000 AAC" ) );
   REQUIRE_MATCHING_OBJECT( voter( "carol", "22.2222 AAC" ), get_voter_info( "carol" ) );
   BOOST_REQUIRE_EQUAL( asset::from_string("2977.7778 AAC"), get_balance( "carol" ) );
   //carol votes for alice
   BOOST_REQUIRE_EQUAL( success(), push_action(N(carol), N(voteproducer), mvo()
                                               ("voter",  "carol")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(alice) } )
                        )
   );
   //new stake votes be added to alice's total_votes
   prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 333333, prod["total_votes"].as_uint64() );

   //bob increases his stake
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "55.0000 AAC", "0.5555 AAC", "0.0000 AAC" ) );
   //should increase alice's total_votes
   prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 888888, prod["total_votes"].as_uint64() );

   //carol unstakes part of the stake
   BOOST_REQUIRE_EQUAL( success(), unstake( "carol", "2.0000 AAC", "0.0002 AAC", 0 ) );
   //should decrease alice's total_votes
   prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 868886, prod["total_votes"].as_uint64() );

   //bob revokes his vote
   BOOST_REQUIRE_EQUAL( success(), push_action( N(bob), N(voteproducer), mvo()
                                               ("voter",  "bob")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>() )
                        )
   );
   //should decrease alice's total_votes
   prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 202220, prod["total_votes"].as_uint64() );
   //but aac should still be at stake
   BOOST_REQUIRE_EQUAL( asset::from_string("1933.3334 AAC"), get_balance( "bob" ) );

   //carol unstakes rest of aac
   BOOST_REQUIRE_EQUAL( success(), unstake( "carol", "20.0000 AAC", "0.2220 AAC", 0 ) );
   //should decrease alice's total_votes to zero
   prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 0, prod["total_votes"].as_uint64() );
   //check that the producer parameters stay the same after all
   BOOST_REQUIRE_EQUAL( "alice", prod["owner"].as_string() );
   REQUIRE_MATCHING_OBJECT( params, prod["prefs"]);
   BOOST_REQUIRE_EQUAL( string(key.begin(), key.end()), to_string(prod["packed_key"]) );
   //carol should receive funds in 3 days
   produce_block( fc::days(3) );
   produce_block();
   BOOST_REQUIRE_EQUAL( asset::from_string("3000.0000 AAC"), get_balance( "carol" ) );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( unregistered_producer_voting, aacio_system_tester ) try {
   issue( "bob", "2000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "13.0000 AAC", "0.5791 AAC", "0.0000 AAC" ) );
   REQUIRE_MATCHING_OBJECT( voter( "bob", "13.5791 AAC" ), get_voter_info( "bob" ) );

   //bob should not be able to vote for alice who is not a producer
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: producer is not registered" ),
                        push_action( N(bob), N(voteproducer), mvo()
                                    ("voter",  "bob")
                                    ("proxy", name(0).to_string() )
                                    ("producers", vector<account_name>{ N(alice) } )
                        )
   );

   //alice registers as a producer
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   fc::variant params = producer_parameters_example(1);
   vector<char> key = fc::raw::pack( get_public_key( N(alice), "active" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproducer), mvo()
                                               ("producer",  "alice")
                                               ("producer_key", key )
                                               ("prefs", params)
                        )
   );
   //and then unregisters
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(unregprod), mvo()
                                               ("producer",  "alice")
                        )
   );
   //key should be empty
   auto prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( true, to_string(prod["packed_key"]).empty() );

   //bob should not be able to vote for alice who is an unregistered producer
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: producer is not currently registered" ),
                        push_action( N(bob), N(voteproducer), mvo()
                                    ("voter",  "bob")
                                    ("proxy", name(0).to_string() )
                                    ("producers", vector<account_name>{ N(alice) } )
                        )
   );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( more_than_30_producer_voting, aacio_system_tester ) try {
   issue( "bob", "2000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "13.0000 AAC", "0.5791 AAC", "0.0000 AAC" ) );
   REQUIRE_MATCHING_OBJECT( voter( "bob", "13.5791 AAC" ), get_voter_info( "bob" ) );

   //bob should not be able to vote for alice who is not a producer
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: attempt to vote for too many producers" ),
                        push_action( N(bob), N(voteproducer), mvo()
                                     ("voter",  "bob")
                                     ("proxy", name(0).to_string() )
                                     ("producers", vector<account_name>(31, N(alice)) )
                        )
   );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( vote_same_producer_30_times, aacio_system_tester ) try {
   issue( "bob", "2000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "50.0000 AAC", "50.0000 AAC", "0.0000 AAC" ) );
   REQUIRE_MATCHING_OBJECT( voter( "bob", "100.0000 AAC" ), get_voter_info( "bob" ) );

   //alice becomes a producer
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   fc::variant params = producer_parameters_example(1);
   vector<char> key = fc::raw::pack( get_public_key( N(alice), "active" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproducer), mvo()
                                               ("producer",  "alice")
                                               ("producer_key", key )
                                               ("prefs", params)
                        )
   );

   //bob should not be able to vote for alice who is not a producer
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: producer votes must be unique and sorted" ),
                        push_action( N(bob), N(voteproducer), mvo()
                                     ("voter",  "bob")
                                     ("proxy", name(0).to_string() )
                                     ("producers", vector<account_name>(30, N(alice)) )
                        )
   );

   auto prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 0, prod["total_votes"].as_uint64() );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( producer_keep_votes, aacio_system_tester ) try {
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   fc::variant params = producer_parameters_example(1);
   vector<char> key = fc::raw::pack( get_public_key( N(alice), "active" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproducer), mvo()
                                               ("producer",  "alice")
                                               ("producer_key", key )
                                               ("prefs", params)
                        )
   );

   //bob makes stake
   issue( "bob", "2000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "13.0000 AAC", "0.5791 AAC", "0.0000 AAC" ) );
   REQUIRE_MATCHING_OBJECT( voter( "bob", "13.5791 AAC" ), get_voter_info( "bob" ) );

   //bob votes for alice
   BOOST_REQUIRE_EQUAL( success(), push_action(N(bob), N(voteproducer), mvo()
                                               ("voter",  "bob")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(alice) } )
                        )
   );

   auto prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 135791, prod["total_votes"].as_uint64() );

   //unregister producer
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(unregprod), mvo()
                                               ("producer",  "alice")
                        )
   );
   prod = get_producer_info( "alice" );
   //key should be empty
   BOOST_REQUIRE_EQUAL( true, to_string(prod["packed_key"]).empty() );
   //check parameters just in case
   REQUIRE_MATCHING_OBJECT( params, prod["prefs"]);
   //votes should stay the same
   BOOST_REQUIRE_EQUAL( 135791, prod["total_votes"].as_uint64() );

   //regtister the same producer again
   params = producer_parameters_example(2);
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproducer), mvo()
                                               ("producer",  "alice")
                                               ("producer_key", key )
                                               ("prefs", params)
                        )
   );
   prod = get_producer_info( "alice" );
   //votes should stay the same
   BOOST_REQUIRE_EQUAL( 135791, prod["total_votes"].as_uint64() );

   //change parameters
   params = producer_parameters_example(3);
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproducer), mvo()
                                               ("producer",  "alice")
                                               ("producer_key", key )
                                               ("prefs", params)
                        )
   );
   prod = get_producer_info( "alice" );
   //votes should stay the same
   BOOST_REQUIRE_EQUAL( 135791, prod["total_votes"].as_uint64() );
   //check parameters just in case
   REQUIRE_MATCHING_OBJECT( params, prod["prefs"]);
   BOOST_REQUIRE_EQUAL( string(key.begin(), key.end()), to_string(prod["packed_key"]) );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( vote_for_two_producers, aacio_system_tester ) try {
   //alice becomes a producer
   fc::variant params = producer_parameters_example(1);
   vector<char> key = fc::raw::pack( get_public_key( N(alice), "active" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproducer), mvo()
                                               ("producer",  "alice")
                                               ("producer_key", key )
                                               ("prefs", params)
                        )
   );
   //bob becomes a producer
   params = producer_parameters_example(2);
   key = fc::raw::pack( get_public_key( N(bob), "active" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(bob), N(regproducer), mvo()
                                               ("producer",  "bob")
                                               ("producer_key", key )
                                               ("prefs", params)
                        )
   );

   //carol votes for alice and bob
   issue( "carol", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "carol", "15.0005 AAC", "5.0000 AAC", "0.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action(N(carol), N(voteproducer), mvo()
                                               ("voter",  "carol")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(alice), N(bob) } )
                        )
   );

   auto alice_info = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 200005, alice_info["total_votes"].as_uint64() );
   auto bob_info = get_producer_info( "bob" );
   BOOST_REQUIRE_EQUAL( 200005, bob_info["total_votes"].as_uint64() );

   //carol votes for alice (but revokes vote for bob)
   BOOST_REQUIRE_EQUAL( success(), push_action(N(carol), N(voteproducer), mvo()
                                               ("voter",  "carol")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(alice) } )
                        )
   );

   alice_info = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 200005, alice_info["total_votes"].as_uint64() );
   bob_info = get_producer_info( "bob" );
   BOOST_REQUIRE_EQUAL( 0, bob_info["total_votes"].as_uint64() );

   //alice votes for herself and bob
   issue( "alice", "2.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "1.0000 AAC", "1.0000 AAC", "0.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(voteproducer), mvo()
                                               ("voter",  "alice")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(alice), N(bob) } )
                        )
   );

   alice_info = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL( 220005, alice_info["total_votes"].as_uint64() );
   bob_info = get_producer_info( "bob" );
   BOOST_REQUIRE_EQUAL( 20000, bob_info["total_votes"].as_uint64() );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( proxy_register_unregister_keeps_stake, aacio_system_tester ) try {
   //register proxy by first action for this user ever
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(regproxy), mvo()
                                               ("proxy",  "alice")
                        )
   );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" ), get_voter_info( "alice" ) );

   //unregister proxy
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(unregproxy), mvo()
                                               ("proxy",  "alice")
                        )
   );
   REQUIRE_MATCHING_OBJECT( voter( "alice" ), get_voter_info( "alice" ) );

   //stake and then register as a proxy
   issue( "bob", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "200.0002 AAC", "100.0001 AAC", "80.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(bob), N(regproxy), mvo()
                                               ("proxy",  "bob")
                        )
   );
   REQUIRE_MATCHING_OBJECT( proxy( "bob" )( "staked", M("300.0003 AAC") ), get_voter_info( "bob" ) );
   //unrgister and check that stake is still in place
   BOOST_REQUIRE_EQUAL( success(), push_action( N(bob), N(unregproxy), mvo()
                                               ("proxy",  "bob")
                        )
   );
   REQUIRE_MATCHING_OBJECT( voter( "bob", "300.0003 AAC" ), get_voter_info( "bob" ) );

   //register as a proxy and then stake
   BOOST_REQUIRE_EQUAL( success(), push_action( N(carol), N(regproxy), mvo()
                                               ("proxy",  "carol")
                        )
   );
   issue( "carol", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "carol", "246.0002 AAC", "531.0001 AAC", "80.0000 AAC" ) );
   //check that both proxy flag and stake a correct
   REQUIRE_MATCHING_OBJECT( proxy( "carol" )( "staked", M("777.0003 AAC") ), get_voter_info( "carol" ) );

   //unregister
   BOOST_REQUIRE_EQUAL( success(), push_action( N(carol), N(unregproxy), mvo()
                                                ("proxy",  "carol")
                        )
   );
   REQUIRE_MATCHING_OBJECT( voter( "carol", "777.0003 AAC" ), get_voter_info( "carol" ) );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( proxy_stake_unstake_keeps_proxy_flag, aacio_system_tester ) try {
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproxy), mvo()
                                               ("proxy",  "alice")
                        )
   );
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" ), get_voter_info( "alice" ) );

   //stake
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "100.0000 AAC", "50.0000 AAC", "80.0000 AAC" ) );
   //check that account is still a proxy
   REQUIRE_MATCHING_OBJECT( proxy( "alice" )( "staked", M("150.0000 AAC") ), get_voter_info( "alice" ) );

   //stake more
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "30.0000 AAC", "20.0000 AAC", "80.0000 AAC" ) );
   //check that account is still a proxy
   REQUIRE_MATCHING_OBJECT( proxy( "alice" )("staked", M("200.0000 AAC") ), get_voter_info( "alice" ) );

   //unstake more
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice", "65.0000 AAC", "35.0000 AAC", 0 ) );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" )("staked", M("100.0000 AAC") ), get_voter_info( "alice" ) );

   //unstake the rest
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice", "65.0000 AAC", "35.0000 AAC", 0 ) );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" )( "staked", M("0.0000 AAC") ), get_voter_info( "alice" ) );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( proxy_actions_affect_producers, aacio_system_tester ) try {
   create_accounts( {  N(producer1), N(producer2), N(producer3) } );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "producer1", 1) );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "producer2", 2) );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "producer3", 3) );

   //register as a proxy
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproxy), mvo()
                                                ("proxy",  "alice")
                        )
   );

   //accumulate proxied votes
   issue( "bob", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "100.0002 AAC", "50.0001 AAC", "50.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action(N(bob), N(voteproducer), mvo()
                                               ("voter",  "bob")
                                               ("proxy", "alice" )
                                               ("producers", vector<account_name>() )
                        )
   );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" )( "proxied_votes", 1500003 ), get_voter_info( "alice" ) );

   //vote for producers
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(voteproducer), mvo()
                                               ("voter",  "alice")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(producer1), N(producer2) } )
                        )
   );
   BOOST_REQUIRE_EQUAL( 1500003, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 1500003, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   //vote for another producers
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(voteproducer), mvo()
                                               ("voter",  "alice")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(producer1), N(producer3) } )
                        )
   );
   BOOST_REQUIRE_EQUAL( 1500003, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 1500003, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   //unregister proxy
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(unregproxy), mvo()
                                                ("proxy",  "alice")
                        )
   );
   //REQUIRE_MATCHING_OBJECT( voter( "alice" )( "proxied_votes", 1500003 ), get_voter_info( "alice" ) );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   //register proxy again
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproxy), mvo()
                                                ("proxy",  "alice")
                        )
   );
   BOOST_REQUIRE_EQUAL( 1500003, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 1500003, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   //stake increase by proxy itself affects producers
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "30.0001 AAC", "20.0001 AAC", "50.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( 2000005, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 2000005, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   //stake decrease by proxy itself affects producers
   BOOST_REQUIRE_EQUAL( success(), unstake( "alice", "10.0001 AAC", "10.0001 AAC", 0 ) );
   BOOST_REQUIRE_EQUAL( 1800003, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 1800003, get_producer_info( "producer3" )["total_votes"].as_uint64() );

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE(producer_pay, aacio_system_tester) try {
   issue( "alice", "10000000.0000 AAC",  config::system_account_name );
   fc::variant params = producer_parameters_example(50);
   vector<char> key = fc::raw::pack(get_public_key(N(alice), "active"));

   // 1 block produced
   BOOST_REQUIRE_EQUAL(success(), push_action(N(alice), N(regproducer), mvo()
                                              ("producer",  "alice")
                                              ("producer_key", key )
                                              ("prefs", params)
                                              )
                       );

   auto prod = get_producer_info( "alice" );
   BOOST_REQUIRE_EQUAL("alice", prod["owner"].as_string());
   BOOST_REQUIRE_EQUAL(0, prod["total_votes"].as_uint64());
   REQUIRE_EQUAL_OBJECTS(params, prod["prefs"]);
   BOOST_REQUIRE_EQUAL(string(key.begin(), key.end()), to_string(prod["packed_key"]));

   issue("bob", "2000.0000 AAC", config::system_account_name);

   // bob makes stake
   // 1 block produced
   BOOST_REQUIRE_EQUAL(success(), push_action(N(bob), N(delegatebw), mvo()
                                              ("from",     "bob")
                                              ("receiver", "bob")
                                              ("stake_net", "11.0000 AAC")
                                              ("stake_cpu", "00.1111 AAC")
                                              ("stake_storage", "0.0000 AAC")
                                              )
                       );

   // bob votes for alice
   // 1 block produced
   BOOST_REQUIRE_EQUAL(success(), push_action(N(bob), N(voteproducer), mvo()
                                              ("voter",  "bob")
                                              ("proxy", name(0).to_string())
                                              ("producers", vector<account_name>{ N(alice) })
                                              )
                       );

   produce_blocks(10);
   prod = get_producer_info("alice");
   BOOST_REQUIRE(prod["per_block_payments"].as_uint64() > 0);

   BOOST_REQUIRE_EQUAL(success(), push_action(N(alice), N(claimrewards), mvo()
                                              ("owner",     "alice")
                                              )
                       );

   prod = get_producer_info("alice");
   BOOST_REQUIRE_EQUAL(0, prod["per_block_payments"].as_uint64());

 } FC_LOG_AND_RETHROW()



BOOST_FIXTURE_TEST_CASE( voters_actions_affect_proxy_and_producers, aacio_system_tester ) try {
   create_accounts( { N(donald), N(producer1), N(producer2), N(producer3) } );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "producer1", 1) );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "producer2", 2) );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "producer3", 3) );

   //alice becomes a producer
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproxy), mvo()
                                                ("proxy",  "alice")
                        )
   );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" ), get_voter_info( "alice" ) );

   //alice makes stake and votes
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "30.0001 AAC", "20.0001 AAC", "50.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(voteproducer), mvo()
                                               ("voter",  "alice")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(producer1), N(producer2) } )
                        )
   );
   BOOST_REQUIRE_EQUAL( 500002, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 500002, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   BOOST_REQUIRE_EQUAL( success(), push_action( N(donald), N(regproxy), mvo()
                                                ("proxy",  "donald")
                        )
   );
   REQUIRE_MATCHING_OBJECT( proxy( "donald" ), get_voter_info( "donald" ) );

   //bob chooses alice as a proxy
   issue( "bob", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "100.0002 AAC", "50.0001 AAC", "50.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(bob), N(voteproducer), mvo()
                                                ("voter",  "bob")
                                                ("proxy", "alice" )
                                                ("producers", vector<account_name>() )
                        )
   );
   BOOST_REQUIRE_EQUAL( 1500003, get_voter_info( "alice" )["proxied_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 2000005, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 2000005, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   //carol chooses alice as a proxy
   issue( "carol", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "carol", "30.0001 AAC", "20.0001 AAC", "80.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(carol), N(voteproducer), mvo()
                                                ("voter",  "carol")
                                                ("proxy", "alice" )
                                                ("producers", vector<account_name>() )
                        )
   );
   BOOST_REQUIRE_EQUAL( 2000005, get_voter_info( "alice" )["proxied_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 2500007, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 2500007, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer3" )["total_votes"].as_uint64() );


   //proxied voter carol increases stake
   BOOST_REQUIRE_EQUAL( success(), stake( "carol", "50.0000 AAC", "70.0000 AAC", "0.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( 3200005, get_voter_info( "alice" )["proxied_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 3700007, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 3700007, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   //proxied voter bob decreases stake
   BOOST_REQUIRE_EQUAL( success(), unstake( "bob", "50.0001 AAC", "50.0001 AAC", 0 ) );
   BOOST_REQUIRE_EQUAL( 2200003, get_voter_info( "alice" )["proxied_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 2700005, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 2700005, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   //proxied voter carol chooses another proxy
   BOOST_REQUIRE_EQUAL( success(), push_action( N(carol), N(voteproducer), mvo()
                                                ("voter",  "carol")
                                                ("proxy", "donald" )
                                                ("producers", vector<account_name>() )
                        )
   );
   BOOST_REQUIRE_EQUAL( 500001, get_voter_info( "alice" )["proxied_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 1700002, get_voter_info( "donald" )["proxied_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 1000003, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 1000003, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer3" )["total_votes"].as_uint64() );

   //bob switches to direct voting and votes for one of the same producers, but not for another one
   BOOST_REQUIRE_EQUAL( success(), push_action( N(bob), N(voteproducer), mvo()
                                                ("voter",  "bob")
                                                ("proxy", "")
                                                ("producers", vector<account_name>{ N(producer2) } )
                        )
   );
   BOOST_REQUIRE_EQUAL( 0, get_voter_info( "alice" )["proxied_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL(  500002, get_producer_info( "producer1" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 1000003, get_producer_info( "producer2" )["total_votes"].as_uint64() );
   BOOST_REQUIRE_EQUAL( 0, get_producer_info( "producer3" )["total_votes"].as_uint64() );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( vote_both_proxy_and_producers, aacio_system_tester ) try {
   //alice becomes a proxy
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproxy), mvo()
                                                ("proxy",  "alice")
                        )
   );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" ), get_voter_info( "alice" ) );

   //carol becomes a producer
   BOOST_REQUIRE_EQUAL( success(), regproducer( "carol", 1) );

   //bob chooses alice as a proxy
   issue( "bob", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "100.0002 AAC", "50.0001 AAC", "50.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( error("condition: assertion failed: cannot vote for producers and proxy at same time"),
                        push_action( N(bob), N(voteproducer), mvo()
                                     ("voter",  "bob")
                                     ("proxy", "alice" )
                                     ("producers", vector<account_name>{ N(carol) } )
                        )
   );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( select_invalid_proxy, aacio_system_tester ) try {
   //accumulate proxied votes
   issue( "bob", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "100.0002 AAC", "50.0001 AAC", "50.0000 AAC" ) );

   //selecting account not registered as a proxy
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: proxy not found" ),
                        push_action(N(bob), N(voteproducer), mvo()
                                    ("voter",  "bob")
                                    ("proxy", "alice" )
                                    ("producers", vector<account_name>() )
                        )
   );

   //selecting not existing account as a proxy
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: proxy not found" ),
                        push_action(N(bob), N(voteproducer), mvo()
                                    ("voter",  "bob")
                                    ("proxy", "notexist" )
                                    ("producers", vector<account_name>() )
                        )
   );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( double_register_unregister_proxy_keeps_votes, aacio_system_tester ) try {
   //alice becomes a proxy
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproxy), mvo()
                                                ("proxy",  "alice")
                        )
   );
   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "5.0000 AAC", "5.0000 AAC", "50.0000 AAC" ) );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" )( "staked", M("10.0000 AAC" ) ), get_voter_info( "alice" ) );

   //bob stakes and selects alice as a proxy
   issue( "bob", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "100.0002 AAC", "50.0001 AAC", "50.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(bob), N(voteproducer), mvo()
                                                ("voter",  "bob")
                                                ("proxy", "alice" )
                                                ("producers", vector<account_name>() )
                        )
   );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" )( "proxied_votes", 1500003 )( "staked", M("10.0000 AAC" ) ), get_voter_info( "alice" ) );

   //double regestering should fail without affecting total votes and stake
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: account is already a proxy" ),
                        push_action( N(alice), N(regproxy), mvo()
                                     ("proxy",  "alice")
                        )
   );
   REQUIRE_MATCHING_OBJECT( proxy( "alice" )( "proxied_votes", 1500003 )( "staked", M("10.0000 AAC" ) ), get_voter_info( "alice" ) );

   //uregister
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(unregproxy), mvo()
                                                ("proxy",  "alice")
                        )
   );
   REQUIRE_MATCHING_OBJECT( voter( "alice" )( "proxied_votes", 1500003 )( "staked", M("10.0000 AAC" ) ), get_voter_info( "alice" ) );

   //double unregistering should not affect proxied_votes and stake
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: account is not a proxy" ),
                        push_action( N(alice), N(unregproxy), mvo()
                                     ("proxy",  "alice")
                        )
   );
   REQUIRE_MATCHING_OBJECT( voter( "alice" )( "proxied_votes", 1500003 )( "staked", M("10.0000 AAC" ) ), get_voter_info( "alice" ) );

} FC_LOG_AND_RETHROW()


BOOST_FIXTURE_TEST_CASE( proxy_cannot_use_another_proxy, aacio_system_tester ) try {
   //alice becomes a proxy
   BOOST_REQUIRE_EQUAL( success(), push_action( N(alice), N(regproxy), mvo()
                                                ("proxy",  "alice")
                        )
   );

   //bob becomes a proxy
   BOOST_REQUIRE_EQUAL( success(), push_action( N(bob), N(regproxy), mvo()
                                                ("proxy",  "bob")
                        )
   );
   //proxy should not be able to use a proxy
   issue( "bob", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "100.0002 AAC", "50.0001 AAC", "50.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: account registered as a proxy is not allowed to use a proxy" ),
                        push_action( N(bob), N(voteproducer), mvo()
                                     ("voter",  "bob")
                                     ("proxy", "alice" )
                                     ("producers", vector<account_name>() )
                        )
   );

   //voter that uses a proxy should not be allowed to become a proxy
   issue( "carol", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "carol", "100.0002 AAC", "50.0001 AAC", "50.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action( N(carol), N(voteproducer), mvo()
                                                ("voter",  "carol")
                                                ("proxy", "alice" )
                                                ("producers", vector<account_name>() )
                        )
   );
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: account that uses a proxy is not allowed to become a proxy" ),
                        push_action( N(carol), N(regproxy), mvo()
                                     ("proxy",  "carol")
                        )
   );

   //proxy should not be able to use itself as a proxy
   BOOST_REQUIRE_EQUAL( error( "condition: assertion failed: account registered as a proxy is not allowed to use a proxy" ),
                        push_action( N(bob), N(voteproducer), mvo()
                                     ("voter",  "bob")
                                     ("proxy", "bob" )
                                     ("producers", vector<account_name>() )
                        )
   );

} FC_LOG_AND_RETHROW()

fc::mutable_variant_object config_to_variant( const aacio::chain::chain_config& config ) {
   return mutable_variant_object()
      ( "base_per_transaction_net_usage", config.base_per_transaction_net_usage )
      ( "base_per_transaction_cpu_usage", config.base_per_transaction_cpu_usage )
      ( "base_per_action_cpu_usage", config.base_per_action_cpu_usage )
      ( "base_setcode_cpu_usage", config.base_setcode_cpu_usage )
      ( "per_signature_cpu_usage", config.per_signature_cpu_usage )
      ( "per_lock_net_usage", config.per_lock_net_usage )
      ( "context_free_discount_cpu_usage_num", config.context_free_discount_cpu_usage_num )
      ( "context_free_discount_cpu_usage_den", config.context_free_discount_cpu_usage_den )
      ( "max_transaction_cpu_usage", config.max_transaction_cpu_usage )
      ( "max_transaction_net_usage", config.max_transaction_net_usage )
      ( "max_block_cpu_usage", config.max_block_cpu_usage )
      ( "target_block_cpu_usage_pct", config.target_block_cpu_usage_pct )
      ( "max_block_net_usage", config.max_block_net_usage )
      ( "target_block_net_usage_pct", config.target_block_net_usage_pct )
      ( "max_transaction_lifetime", config.max_transaction_lifetime )
      ( "max_transaction_exec_time", config.max_transaction_exec_time )
      ( "max_authority_depth", config.max_authority_depth )
      ( "max_inline_depth", config.max_inline_depth )
      ( "max_inline_action_size", config.max_inline_action_size )
      ( "max_generated_transaction_count", config.max_generated_transaction_count );
}

BOOST_FIXTURE_TEST_CASE( elect_producers_and_parameters, aacio_system_tester ) try {
   create_accounts( {  N(producer1), N(producer2), N(producer3) } );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "producer1", 1) );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "producer2", 2) );
   BOOST_REQUIRE_EQUAL( success(), regproducer( "producer3", 3) );

   issue( "alice", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "alice", "100.0000 AAC", "50.0000 AAC", "50.0000 AAC" ) );
   //vote for producers
   BOOST_REQUIRE_EQUAL( success(), push_action(N(alice), N(voteproducer), mvo()
                                               ("voter",  "alice")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(producer1) } )
                        )
   );
   produce_blocks(50);
   auto producer_keys = control->get_global_properties().active_producers.producers;
   BOOST_REQUIRE_EQUAL( 1, producer_keys.size() );
   BOOST_REQUIRE_EQUAL( name("producer1"), producer_keys[0].producer_name );

   auto config = config_to_variant( control->get_global_properties().configuration );
   auto prod1_config = testing::filter_fields( config, producer_parameters_example( 1 ) );
   REQUIRE_EQUAL_OBJECTS(prod1_config, config);

   // elect 2 producers
   issue( "bob", "1000.0000 AAC",  config::system_account_name );
   BOOST_REQUIRE_EQUAL( success(), stake( "bob", "200.0000 AAC", "100.0000 AAC", "50.0000 AAC" ) );
   BOOST_REQUIRE_EQUAL( success(), push_action(N(bob), N(voteproducer), mvo()
                                               ("voter",  "bob")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(producer2) } )
                        )
   );
   produce_blocks(50);
   producer_keys = control->get_global_properties().active_producers.producers;
   BOOST_REQUIRE_EQUAL( 2, producer_keys.size() );
   BOOST_REQUIRE_EQUAL( name("producer2"), producer_keys[0].producer_name );
   BOOST_REQUIRE_EQUAL( name("producer1"), producer_keys[1].producer_name );
   config = config_to_variant( control->get_global_properties().configuration );
   auto prod2_config = testing::filter_fields( config, producer_parameters_example( 2 ) );
   REQUIRE_EQUAL_OBJECTS(prod2_config, config);

   // elect 3 producers
   BOOST_REQUIRE_EQUAL( success(), push_action(N(bob), N(voteproducer), mvo()
                                               ("voter",  "bob")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(producer2), N(producer3) } )
                        )
   );
   produce_blocks(50);
   producer_keys = control->get_global_properties().active_producers.producers;
   BOOST_REQUIRE_EQUAL( 3, producer_keys.size() );
   BOOST_REQUIRE_EQUAL( name("producer3"), producer_keys[0].producer_name );
   BOOST_REQUIRE_EQUAL( name("producer2"), producer_keys[1].producer_name );
   BOOST_REQUIRE_EQUAL( name("producer1"), producer_keys[2].producer_name );
   config = config_to_variant( control->get_global_properties().configuration );
   REQUIRE_EQUAL_OBJECTS(prod2_config, config);

   //back to 2 producers
   BOOST_REQUIRE_EQUAL( success(), push_action(N(bob), N(voteproducer), mvo()
                                               ("voter",  "bob")
                                               ("proxy", name(0).to_string() )
                                               ("producers", vector<account_name>{ N(producer3) } )
                        )
   );
   produce_blocks(100);
   producer_keys = control->get_global_properties().active_producers.producers;
   BOOST_REQUIRE_EQUAL( 2, producer_keys.size() );
   BOOST_REQUIRE_EQUAL( name("producer3"), producer_keys[0].producer_name );
   BOOST_REQUIRE_EQUAL( name("producer1"), producer_keys[1].producer_name );
   config = config_to_variant( control->get_global_properties().configuration );
   auto prod3_config = testing::filter_fields( config, producer_parameters_example( 3 ) );
   REQUIRE_EQUAL_OBJECTS(prod3_config, config);

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
