#include <aaciolib/aacio.hpp>
using namespace aacio;

class hello : public aacio::contract {
  public:
      using contract::contract;

      /// @abi action 
      void hi( account_name user ) {
         print( "Hello, ", name{user} );
      }
};

AACIO_ABI( hello, (hi) )
