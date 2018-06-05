/**
 *  @file
 *  @copyright defined in Acute-Angle-Chain/LICENSE.txt
 */
#pragma once

#include <aaciolib/aacio.hpp>
#include <aaciolib/dispatcher.hpp>

namespace noop {
   using std::string;


   /**
      noop contract
      All it does is require sender authorization.
      Actions: anyaction
    */
   class noop {
      public:
         
         ACTION(N(noop), anyaction) {
            anyaction() { }
            anyaction(account_name f, const string& t, const string& d): from(f), type(t), data(d) { }
            
            account_name from;
            string type;
            string data;
            
            AACLIB_SERIALIZE(anyaction, (from)(type)(data))
         };

         static void on(const anyaction& act)
         {
            require_auth(act.from);
         }
   };
} /// noop
