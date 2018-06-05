/**
 *  @file
 *  @copyright defined in AcuteAngleChain/LICENSE.txt
 */
#pragma once
#include <aacio/chain/types.hpp>
#include <aacio/chain/authority.hpp>
#include <aacio/chain/block_timestamp.hpp>
#include <aacio/chain/contracts/types.hpp>

#include "multi_index_includes.hpp"

namespace aacio { namespace chain {

   class scope_sequence_object : public chainbase::object<scope_sequence_object_type, scope_sequence_object> {
      OBJECT_CTOR(scope_sequence_object)

      id_type              id;
      scope_name           scope;
      account_name         receiver;
      uint64_t             sequence       = 0;
   };
   using scope_sequence_id_type = scope_sequence_object::id_type;

   struct by_scope_receiver;
   using scope_sequence_multi_index = chainbase::shared_multi_index_container<
      scope_sequence_object,
      indexed_by<
         ordered_unique<tag<by_id>, member<scope_sequence_object, scope_sequence_object::id_type, &scope_sequence_object::id>>,
         ordered_unique<tag<by_scope_receiver>,
            composite_key< scope_sequence_object,
               member<scope_sequence_object, scope_name, &scope_sequence_object::scope>,
               member<scope_sequence_object, account_name, &scope_sequence_object::receiver>
            >
         >
      >
   >;

   typedef chainbase::generic_index<scope_sequence_multi_index> scope_sequence_index;

} } // aacio::chain

CHAINBASE_SET_INDEX_TYPE(aacio::chain::scope_sequence_object, aacio::chain::scope_sequence_multi_index)

FC_REFLECT(chainbase::oid<aacio::chain::scope_sequence_object>, (_id))

FC_REFLECT(aacio::chain::scope_sequence_object, (id)(scope)(receiver)(sequence))
