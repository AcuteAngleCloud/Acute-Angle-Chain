#pragma once

#include <aaciolib/aacio.hpp>

namespace aacio {

   class sudo : public contract {
      public:
         sudo( account_name self ):contract(self){}

         void exec();

   };

} /// namespace aacio
