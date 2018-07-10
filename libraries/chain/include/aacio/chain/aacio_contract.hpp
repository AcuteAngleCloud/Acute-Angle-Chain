/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */
#pragma once

#include <aacio/chain/types.hpp>
#include <aacio/chain/contract_types.hpp>

namespace aacio { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_aacio_newaccount(apply_context&);
   void apply_aacio_updateauth(apply_context&);
   void apply_aacio_deleteauth(apply_context&);
   void apply_aacio_linkauth(apply_context&);
   void apply_aacio_unlinkauth(apply_context&);

   /*
   void apply_aacio_postrecovery(apply_context&);
   void apply_aacio_passrecovery(apply_context&);
   void apply_aacio_vetorecovery(apply_context&);
   */

   void apply_aacio_setcode(apply_context&);
   void apply_aacio_setabi(apply_context&);

   void apply_aacio_canceldelay(apply_context&);
   ///@}  end action handlers

} } /// namespace aacio::chain
