/**
 *  @file
 *  @copyright defined in AcuteAngleChain/LICENSE.txt
 */
#pragma once

#include <fc/exception/exception.hpp>
#include <aacio/chain/exceptions.hpp>

#define AAC_DECLARE_INTERNAL_EXCEPTION( exc_name, seqnum, msg )  \
   FC_DECLARE_DERIVED_EXCEPTION(                                      \
      internal_ ## exc_name,                                          \
      aacio::chain::internal_exception,                            \
      3990000 + seqnum,                                               \
      msg                                                             \
      )

namespace aacio { namespace chain {

FC_DECLARE_DERIVED_EXCEPTION( internal_exception, aacio::chain::chain_exception, 3990000, "internal exception" )

AAC_DECLARE_INTERNAL_EXCEPTION( verify_auth_max_auth_exceeded, 1, "Exceeds max authority fan-out" )
AAC_DECLARE_INTERNAL_EXCEPTION( verify_auth_account_not_found, 2, "Auth account not found" )

} } // aacio::chain
