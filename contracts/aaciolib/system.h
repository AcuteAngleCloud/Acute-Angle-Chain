/**
 *  @file
 *  @copyright defined in AcuteAngleChain/LICENSE.txt
 */
#pragma once
#include <aaciolib/types.h>

extern "C" {

   /**
    * @defgroup systemapi System API
    * @ingroup contractdev
    * @brief Define API for interacting with system level intrinsics
    *
    */

   /**
    * @defgroup systemcapi System C API
    * @ingroup systemapi
    * @brief Define API for interacting with system level intrinsics
    *
    * @{
    */

   /**
    *  Aborts processing of this action and unwinds all pending changes if the test condition is true
    *  @brief Aborts processing of this action and unwinds all pending changes
    *  @param test - 0 to abort, 1 to ignore
    *  @param cstr - a null terminated action to explain the reason for failure

    */
   void  aacio_assert( uint32_t test, const char* cstr );

   /**
    * This method will abort execution of wasm without failing the contract. This
    * is used to bypass all cleanup / destructors that would normally be called.
    */
   [[noreturn]] void  aacio_exit( int32_t code );

   /**
    *  Returns the time in seconds from 1970 of the last accepted block (not the block including this action)
    *  @brief Get time of the last accepted block
    *  @return time in seconds from 1970 of the last accepted block
    */
   time  now();
   ///@ } systemcapi


}

