/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */

#include <aaciolib/aacio.hpp>

namespace aacio {

   class noop: public contract {
      public:
         noop( account_name self ): contract( self ) { }
         void anyaction( account_name from,
                         const std::string& /*type*/,
                         const std::string& /*data*/ )
         {
            require_auth( from );
         }
   };

   AACIO_ABI( noop, ( anyaction ) )

} /// aacio     
