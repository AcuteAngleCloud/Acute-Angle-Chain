#pragma once 
#include <aaciolib/varint.hpp>
#include <aaciolib/serialize.hpp>

namespace aacio {
   struct public_key {
      unsigned_int        type;
      std::array<char,33> data;

      friend bool operator == ( const public_key& a, const public_key& b ) {
        return std::tie(a.type,a.data) == std::tie(b.type,b.data);
      }
      friend bool operator != ( const public_key& a, const public_key& b ) {
        return std::tie(a.type,a.data) != std::tie(b.type,b.data);
      }
      AACLIB_SERIALIZE( public_key, (type)(data) )
   };
}
