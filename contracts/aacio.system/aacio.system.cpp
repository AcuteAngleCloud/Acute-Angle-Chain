/**
 *  @file
 *  @copyright defined in AcuteAngleChain/LICENSE.txt
 */

#include <aacio.system/aacio.system.hpp> 

using namespace aaciosystem;

extern "C" {

    /// The apply method implements the dispatch of events to this contract
    void apply( uint64_t receiver, uint64_t code, uint64_t act ) {
       //print( aacio::name(code), "::", aacio::name(act) );
       aaciosystem::contract<N(aacio)>::apply( receiver, code, act );
    }
}
