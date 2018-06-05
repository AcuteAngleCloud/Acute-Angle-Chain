/**
 *  @file
 *  @copyright defined in Acute-Angle-Chain/LICENSE.txt
 */
#include <aaciolib/action.h>
#include <aaciolib/chain.h>
#include <aaciolib/aacio.hpp>
#include "test_api.hpp"

#pragma pack(push, 1)
struct producers {
   char len;
   account_name producers[31];
};
#pragma pack(pop)

void test_chain::test_activeprods() {
  producers act_prods;
  read_action_data(&act_prods, sizeof(producers));
   
  aacio_assert(act_prods.len == 31, "producers.len != 31");

  producers api_prods;
  get_active_producers(api_prods.producers, sizeof(account_name)*31);

  for( int i = 0; i < 31 ; ++i )
      aacio_assert(api_prods.producers[i] == act_prods.producers[i], "Active producer");
}
