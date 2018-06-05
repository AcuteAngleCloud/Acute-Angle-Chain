#pragma once 
#include <aaciolib/varint.hpp>
#include <aaciolib/serialize.hpp>

namespace aacio {
   struct public_key {
      unsigned_int        type;
      std::array<char,33> data;

      AACLIB_SERIALIZE( public_key, (type)(data) )
   };
}
