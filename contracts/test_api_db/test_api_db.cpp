/**
 *  @file
 *  @copyright defined in AcuteAngleChain/LICENSE.txt
 */
#include <aaciolib/aacio.hpp>
#include "../test_api/test_api.hpp"

#include "test_db.cpp"

extern "C" {
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
      require_auth(code); 
      WASM_TEST_HANDLER_EX(test_db, primary_i64_general);
      WASM_TEST_HANDLER_EX(test_db, primary_i64_lowerbound);
      WASM_TEST_HANDLER_EX(test_db, primary_i64_upperbound);
      WASM_TEST_HANDLER_EX(test_db, idx64_general);
      WASM_TEST_HANDLER_EX(test_db, idx64_lowerbound);
      WASM_TEST_HANDLER_EX(test_db, idx64_upperbound);

      //unhandled test call
      aacio_assert(false, "Unknown Test");
   }

}
