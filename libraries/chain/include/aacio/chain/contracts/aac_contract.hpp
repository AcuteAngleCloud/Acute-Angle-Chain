/**
 *  @file
 *  @copyright defined in AcuteAngleChain/LICENSE.txt
 */
#pragma once

#include <aacio/chain/apply_context.hpp>

#include <aacio/chain/types.hpp>

namespace aacio { namespace chain { namespace contracts {

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_aacio_newaccount(apply_context&);
   void apply_aacio_updateauth(apply_context&);
   void apply_aacio_deleteauth(apply_context&);
   void apply_aacio_linkauth(apply_context&);
   void apply_aacio_unlinkauth(apply_context&);

   void apply_aacio_postrecovery(apply_context&);
   void apply_aacio_passrecovery(apply_context&);
   void apply_aacio_vetorecovery(apply_context&);

   void apply_aacio_setcode(apply_context&);
   void apply_aacio_setabi(apply_context&);

   void apply_aacio_onerror(apply_context&);

   void apply_aacio_canceldelay(apply_context&);
   ///@}  end action handlers

} } } /// namespace aacio::contracts
