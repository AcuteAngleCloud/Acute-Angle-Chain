/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */

#include <aacio/chain/chain_id_type.hpp>
#include <aacio/chain/exceptions.hpp>

namespace aacio { namespace chain {

   void chain_id_type::reflector_verify()const {
      FC_ASSERT( *reinterpret_cast<const fc::sha256*>(this) != fc::sha256(), "chain_id_type cannot be zero" );
   }

} }  // namespace aacio::chain

namespace fc {

   void to_variant(const aacio::chain::chain_id_type& cid, fc::variant& v) {
      to_variant( static_cast<const fc::sha256&>(cid), v);
   }

   void from_variant(const fc::variant& v, aacio::chain::chain_id_type& cid) {
      from_variant( v, static_cast<fc::sha256&>(cid) );
   }

} // fc
