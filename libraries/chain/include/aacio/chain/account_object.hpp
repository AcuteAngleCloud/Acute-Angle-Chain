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

   class account_object : public chainbase::object<account_object_type, account_object> {
      OBJECT_CTOR(account_object,(code)(abi))

      id_type              id;
      account_name         name;
      uint8_t              vm_type      = 0;
      uint8_t              vm_version   = 0;
      bool                 privileged   = false;

      time_point_sec       last_code_update;
      digest_type          code_version;
      block_timestamp_type creation_date;

      shared_vector<char>  code;
      shared_vector<char>  abi;

      void set_abi( const aacio::chain::contracts::abi_def& a ) {
         // Added resize(0) here to avoid bug in boost vector container
         abi.resize( 0 );
         abi.resize( fc::raw::pack_size( a ) );
         fc::datastream<char*> ds( abi.data(), abi.size() );
         fc::raw::pack( ds, a );
      }

      aacio::chain::contracts::abi_def get_abi()const {
         aacio::chain::contracts::abi_def a;
         fc::datastream<const char*> ds( abi.data(), abi.size() );
         fc::raw::unpack( ds, a );
         return a;
      }
   };
   using account_id_type = account_object::id_type;

   struct by_name;
   using account_index = chainbase::shared_multi_index_container<
      account_object,
      indexed_by<
         ordered_unique<tag<by_id>, member<account_object, account_object::id_type, &account_object::id>>,
         ordered_unique<tag<by_name>, member<account_object, account_name, &account_object::name>>
      >
   >;

} } // aacio::chain

CHAINBASE_SET_INDEX_TYPE(aacio::chain::account_object, aacio::chain::account_index)


FC_REFLECT(aacio::chain::account_object, (name)(vm_type)(vm_version)(code_version)(code)(creation_date))
