#pragma once
#include <aaciolib/generic_currency.hpp>

namespace aacio {

/* TODO: make native currency match generic currency, requires changes in the native code
struct native_token_def {
   static const uint64_t code = N(aacio);
   static const uint64_t symbol = N(aac);
};
using native_currency = generic_currency<native_token_def>;
 */

struct native_currency {
   struct transfer : public action_meta<N(aacio),N(transfer)> {
      account_name from;
      account_name to;
      uint64_t     quantity;
      string       memo;

      template<typename DataStream>
      friend DataStream& operator << ( DataStream& ds, const transfer& t ){
         return ds << t.from << t.to << t.quantity << t.memo;
      }

      template<typename DataStream>
      friend DataStream& operator >> ( DataStream& ds,  transfer& t ){
         return ds >> t.from >> t.to >> t.quantity >> t.memo;

      }

   };
};

} /// namespace aacio

