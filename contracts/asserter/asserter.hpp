/**
 *  @file
 *  @copyright defined in Acute-Angle-Chain/LICENSE.txt
 */

#include <aaciolib/aacio.hpp>

namespace asserter {
   struct PACKED(assertdef) {
      int8_t   condition;
      int8_t   message_length;
      char     message[];
   };
}
