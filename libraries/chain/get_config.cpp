/**
 *  @file
 *  @copyright defined in Acute-Angle-Chain/LICENSE.txt
 */

#include <aacio/chain/get_config.hpp>
#include <aacio/chain/config.hpp>
#include <aacio/chain/types.hpp>

namespace aacio { namespace chain {

fc::variant_object get_config()
{
   fc::mutable_variant_object result;

//   result["block_interval_ms"] = config::block_interval_ms;
//   result["producer_count"] = config::producer_count;
   /// TODO: add extra config parms
   return result;
}

} } // aacio::chain
