/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */

#include <aaciolib/aacio.hpp>

namespace asserter {
   struct assertdef {
      int8_t      condition;
      std::string message;

      AACLIB_SERIALIZE( assertdef, (condition)(message) )
   };
}
