#include <aaciolib/aacio.hpp>
#include <aaciolib/print.hpp>
using namespace aacio;

class payloadless : public aacio::contract {
  public:
      using contract::contract;

      void doit() {
         print( "Im a payloadless action" );
      }
};

AACIO_ABI( payloadless, (doit) )
