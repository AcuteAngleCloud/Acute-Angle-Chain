/**
 * @file action_test.cpp
 * @copyright defined in AcuteAngleChain/LICENSE.txt
 */
#include <aaciolib/action.hpp>
#include <aaciolib/transaction.hpp>
#include <aaciolib/chain.h>
#include <aaciolib/db.h>
#include <aaciolib/crypto.h>
#include <aaciolib/privileged.h>
#include <aaciolib/aacio.hpp>
#include <aaciolib/datastream.hpp>
#include <aaciolib/print.hpp>
#include <aaciolib/compiler_builtins.h>

#include "test_api.hpp"
void test_action::read_action_normal() {

   char buffer[100];
   uint32_t total = 0;

   aacio_assert(action_data_size() == sizeof(dummy_action), "action_size() == sizeof(dummy_action)");

   total = read_action_data(buffer, 30);
   aacio_assert(total == sizeof(dummy_action) , "read_action(30)" );

   total = read_action_data(buffer, 100);
   aacio_assert(total == sizeof(dummy_action) , "read_action(100)" );

   total = read_action_data(buffer, 5);
   aacio_assert(total == 5 , "read_action(5)" );

   total = read_action_data(buffer, sizeof(dummy_action) );
   aacio_assert(total == sizeof(dummy_action), "read_action(sizeof(dummy_action))" );

   dummy_action *dummy13 = reinterpret_cast<dummy_action *>(buffer);

   aacio_assert(dummy13->a == DUMMY_ACTION_DEFAULT_A, "dummy13->a == DUMMY_ACTION_DEFAULT_A");
   aacio_assert(dummy13->b == DUMMY_ACTION_DEFAULT_B, "dummy13->b == DUMMY_ACTION_DEFAULT_B");
   aacio_assert(dummy13->c == DUMMY_ACTION_DEFAULT_C, "dummy13->c == DUMMY_ACTION_DEFAULT_C");
}

void test_action::test_dummy_action() {
   char buffer[100];
   int total = 0;

   // get_action
   total = get_action( 1, 0, buffer, 0 );
   total = get_action( 1, 0, buffer, static_cast<size_t>(total) );
   aacio_assert( total > 0, "get_action failed" );
   aacio::action act = aacio::get_action( 1, 0 );
   aacio_assert( aacio::pack_size(act) == static_cast<size_t>(total), "pack_size does not match get_action size" );
   aacio_assert( act.account == N(testapi), "expected testapi account" );

   dummy_action dum13 = act.data_as<dummy_action>();

   if ( dum13.b == 200 ) {
      // attempt to access context free only api
      get_context_free_data( 0, nullptr, 0 );
      aacio_assert(false, "get_context_free_data() not allowed in non-context free action");
   } else {
      aacio_assert(dum13.a == DUMMY_ACTION_DEFAULT_A, "dum13.a == DUMMY_ACTION_DEFAULT_A");
      aacio_assert(dum13.b == DUMMY_ACTION_DEFAULT_B, "dum13.b == DUMMY_ACTION_DEFAULT_B");
      aacio_assert(dum13.c == DUMMY_ACTION_DEFAULT_C, "dum13.c == DUMMY_ACTION_DEFAULT_C");
   }
}

void test_action::read_action_to_0() {
   read_action_data((void *)0, action_data_size());
}

void test_action::read_action_to_64k() {
   read_action_data( (void *)((1<<16)-2), action_data_size());
}

void test_action::test_cf_action() {

   aacio::action act = aacio::get_action( 0, 0 );
   cf_action cfa = act.data_as<cf_action>();

   if ( cfa.payload == 100 ) {
      // verify read of get_context_free_data, also verifies system api access
      int size = get_context_free_data( cfa.cfd_idx, nullptr, 0 );
      aacio_assert( size > 0, "size determination failed" );
      aacio::bytes cfd( static_cast<size_t>(size) );
      size = get_context_free_data( cfa.cfd_idx, &cfd[0], static_cast<size_t>(size) );
      aacio_assert(static_cast<size_t>(size) == cfd.size(), "get_context_free_data failed" );
      uint32_t v = aacio::unpack<uint32_t>( &cfd[0], cfd.size() );
      aacio_assert( v == cfa.payload, "invalid value" );

      // verify crypto api access
      checksum256 hash;
      char test[] = "test";
      sha256( test, sizeof(test), &hash );
      assert_sha256( test, sizeof(test), &hash );
      // verify action api access
      action_data_size();
      // verify console api access
      aacio::print("test\n");
      // verify memory api access
      uint32_t i = 42;
      memccpy(&v, &i, sizeof(i), sizeof(i));
      // verify transaction api access
      aacio_assert(transaction_size() > 0, "transaction_size failed");

   } else if ( cfa.payload == 200 ) {
      // attempt to access non context free api, privileged_api
      is_privileged(act.name);
      aacio_assert( false, "privileged_api should not be allowed" );
   } else if ( cfa.payload == 201 ) {
      // attempt to access non context free api, producer_api
      get_active_producers( nullptr, 0 );
      aacio_assert( false, "producer_api should not be allowed" );
   } else if ( cfa.payload == 202 ) {
      // attempt to access non context free api, db_api
      db_store_i64( N(testapi), N(testapi), N(testapi), 0, "test", 4 );
      aacio_assert( false, "db_api should not be allowed" );
   } else if ( cfa.payload == 203 ) {
      // attempt to access non context free api, db_api
      uint64_t i = 0;
      db_idx64_store( N(testapi), N(testapi), N(testapi), 0, &i );
      aacio_assert( false, "db_api should not be allowed" );
   } else if ( cfa.payload == 204 ) {
      // attempt to access non context free api, send action
      aacio::action dum_act;
      dum_act.send();
      aacio_assert( false, "action send should not be allowed" );
   }

}

void test_action::require_notice(uint64_t receiver, uint64_t code, uint64_t action) {
   (void)code;(void)action;
   if( receiver == N(testapi) ) {
      aacio::require_recipient( N(acc1) );
      aacio::require_recipient( N(acc2) );
      aacio::require_recipient( N(acc1), N(acc2) );
      aacio_assert(false, "Should've failed");
   } else if ( receiver == N(acc1) || receiver == N(acc2) ) {
      return;
   }
   aacio_assert(false, "Should've failed");
}

void test_action::require_auth() {
   prints("require_auth");
   aacio::require_auth( N(acc3) );
   aacio::require_auth( N(acc4) );
}

void test_action::assert_false() {
   aacio_assert(false, "test_action::assert_false");
}

void test_action::assert_true() {
   aacio_assert(true, "test_action::assert_true");
}

void test_action::assert_true_cf() {
   aacio_assert(true, "test_action::assert_true");
}

void test_action::test_abort() {
   abort();
   aacio_assert( false, "should've aborted" );
}

void test_action::test_publication_time() {
   uint32_t pub_time = 0;
   read_action_data(&pub_time, sizeof(uint32_t));
   aacio_assert( pub_time == publication_time(), "pub_time == publication_time()" );
}

void test_action::test_current_receiver(uint64_t receiver, uint64_t code, uint64_t action) {
   (void)code;(void)action;
   account_name cur_rec;
   read_action_data(&cur_rec, sizeof(account_name));

   aacio_assert( receiver == cur_rec, "the current receiver does not match" );
}

void test_action::test_current_sender() {
   account_name cur_send;
   read_action_data(&cur_send, sizeof(account_name));
   aacio_assert( current_sender() == cur_send, "the current sender does not match" );
}

void test_action::now() {
   uint32_t tmp = 0;
   uint32_t total = read_action_data(&tmp, sizeof(uint32_t));
   aacio_assert( total == sizeof(uint32_t), "total == sizeof(uint32_t)");
   aacio_assert( tmp == ::now(), "tmp == now()" );
}
