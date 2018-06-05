/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */
#pragma once
#include "common.hpp"

#include <aaciolib/aacio.hpp>
#include <aaciolib/token.hpp>
#include <aaciolib/print.hpp>

#include <aaciolib/generic_currency.hpp>
#include <aaciolib/datastream.hpp>
#include <aaciolib/serialize.hpp>
#include <aaciolib/multi_index.hpp>
#include <aaciolib/privileged.hpp>
#include <aaciolib/singleton.hpp>
#include <aaciolib/transaction.hpp>

#include <algorithm>
#include <array>
#include <cmath>

namespace aaciosystem {
   using aacio::indexed_by;
   using aacio::const_mem_fun;
   using aacio::bytes;
   using aacio::print;
   using aacio::singleton;
   using aacio::transaction;


   template<account_name SystemAccount>
   class voting {
      public:
         static constexpr account_name system_account = SystemAccount;
         using currency = typename common<SystemAccount>::currency;
         using system_token_type = typename common<SystemAccount>::system_token_type;
         using aacio_parameters = typename common<SystemAccount>::aacio_parameters;
         using global_state_singleton = typename common<SystemAccount>::global_state_singleton;

         static constexpr uint32_t max_inflation_rate = common<SystemAccount>::max_inflation_rate;
         static constexpr uint32_t blocks_per_year = 52*7*24*2*3600; // half seconds per year

         struct producer_info {
            account_name      owner;
            uint128_t         total_votes = 0;
            aacio_parameters  prefs;
            aacio::bytes      packed_key; /// a packed public key object
            system_token_type per_block_payments;
            time              last_rewards_claim = 0;
            time              time_became_active = 0;
            time              last_produced_block_time = 0;

            uint64_t    primary_key()const { return owner;       }
            uint128_t   by_votes()const    { return total_votes; }
            bool active() const { return packed_key.size() == sizeof(public_key); }

            AACLIB_SERIALIZE( producer_info, (owner)(total_votes)(prefs)(packed_key)
                              (per_block_payments)(last_rewards_claim)
                              (time_became_active)(last_produced_block_time) )
         };

         typedef aacio::multi_index< N(producerinfo), producer_info,
                                     indexed_by<N(prototalvote), const_mem_fun<producer_info, uint128_t, &producer_info::by_votes>  >
                                     >  producers_table;


         struct voter_info {
            account_name                owner = 0;
            account_name                proxy = 0;
            time                        last_update = 0;
            uint32_t                    is_proxy = 0;
            system_token_type           staked;
            system_token_type           unstaking;
            system_token_type           unstake_per_week;
            uint128_t                   proxied_votes = 0;
            std::vector<account_name>   producers;
            uint32_t                    deferred_trx_id = 0;
            time                        last_unstake_time = 0; //uint32

            uint64_t primary_key()const { return owner; }

            AACLIB_SERIALIZE( voter_info, (owner)(proxy)(last_update)(is_proxy)(staked)(unstaking)(unstake_per_week)(proxied_votes)(producers)(deferred_trx_id)(last_unstake_time) )
         };

         typedef aacio::multi_index< N(voters), voter_info>  voters_table;

         ACTION( SystemAccount, regproducer ) {
            account_name     producer;
            bytes            producer_key;
            aacio_parameters prefs;

            AACLIB_SERIALIZE( regproducer, (producer)(producer_key)(prefs) )
         };

         /**
          *  This method will create a producer_config and producer_info object for 'producer'
          *
          *  @pre producer is not already registered
          *  @pre producer to register is an account
          *  @pre authority of producer to register
          *
          */
         static void on( const regproducer& reg ) {
            require_auth( reg.producer );

            producers_table producers_tbl( SystemAccount, SystemAccount );
            auto prod = producers_tbl.find( reg.producer );

            if ( prod != producers_tbl.end() ) {
               producers_tbl.modify( prod, reg.producer, [&]( producer_info& info ){
                     info.prefs = reg.prefs;
                     info.packed_key = reg.producer_key;
                  });
            } else {
               producers_tbl.emplace( reg.producer, [&]( producer_info& info ){
                     info.owner       = reg.producer;
                     info.total_votes = 0;
                     info.prefs       = reg.prefs;
                     info.packed_key  = reg.producer_key;
                  });
            }
         }

         ACTION( SystemAccount, unregprod ) {
            account_name producer;

            AACLIB_SERIALIZE( unregprod, (producer) )
         };

         static void on( const unregprod& unreg ) {
            require_auth( unreg.producer );

            producers_table producers_tbl( SystemAccount, SystemAccount );
            auto prod = producers_tbl.find( unreg.producer );
            aacio_assert( prod != producers_tbl.end(), "producer not found" );

            producers_tbl.modify( prod, 0, [&]( producer_info& info ){
                  info.packed_key.clear();
               });
         }

         static void increase_voting_power( account_name acnt, system_token_type amount ) {
            voters_table voters_tbl( SystemAccount, SystemAccount );
            auto voter = voters_tbl.find( acnt );

            if( voter == voters_tbl.end() ) {
               voter = voters_tbl.emplace( acnt, [&]( voter_info& a ) {
                     a.owner = acnt;
                     a.last_update = now();
                     a.staked = amount;
                  });
            } else {
               voters_tbl.modify( voter, 0, [&]( auto& av ) {
                     av.last_update = now();
                     av.staked += amount;
                  });
            }

            const std::vector<account_name>* producers = nullptr;
            if ( voter->proxy ) {
               auto proxy = voters_tbl.find( voter->proxy );
               aacio_assert( proxy != voters_tbl.end(), "selected proxy not found" ); //data corruption
               voters_tbl.modify( proxy, 0, [&](voter_info& a) { a.proxied_votes += amount.quantity; } );
               if ( proxy->is_proxy ) { //only if proxy is still active. if proxy has been unregistered, we update proxied_votes, but don't propagate to producers
                  producers = &proxy->producers;
               }
            } else {
               producers = &voter->producers;
            }

            if ( producers ) {
               producers_table producers_tbl( SystemAccount, SystemAccount );
               for( auto p : *producers ) {
                  auto prod = producers_tbl.find( p );
                  aacio_assert( prod != producers_tbl.end(), "never existed producer" ); //data corruption
                  producers_tbl.modify( prod, 0, [&]( auto& v ) {
                        v.total_votes += amount.quantity;
                     });
               }
            }
         }

         static void decrease_voting_power( account_name acnt, system_token_type amount ) {
            require_auth( acnt );
            voters_table voters_tbl( SystemAccount, SystemAccount );
            auto voter = voters_tbl.find( acnt );
            aacio_assert( voter != voters_tbl.end(), "stake not found" );

            if ( 0 < amount.quantity ) {
               aacio_assert( amount <= voter->staked, "cannot unstake more than total stake amount" );
               voters_tbl.modify( voter, 0, [&](voter_info& a) {
                     a.staked -= amount;
                     a.last_update = now();
                  });

               const std::vector<account_name>* producers = nullptr;
               if ( voter->proxy ) {
                  auto proxy = voters_tbl.find( voter->proxy );
                  voters_tbl.modify( proxy, 0, [&](voter_info& a) { a.proxied_votes -= amount.quantity; } );
                  if ( proxy->is_proxy ) { //only if proxy is still active. if proxy has been unregistered, we update proxied_votes, but don't propagate to producers
                     producers = &proxy->producers;
                  }
               } else {
                  producers = &voter->producers;
               }

               if ( producers ) {
                  producers_table producers_tbl( SystemAccount, SystemAccount );
                  for( auto p : *producers ) {
                     auto prod = producers_tbl.find( p );
                     aacio_assert( prod != producers_tbl.end(), "never existed producer" ); //data corruption
                     producers_tbl.modify( prod, 0, [&]( auto& v ) {
                           v.total_votes -= amount.quantity;
                        });
                  }
               }
            } else {
               if (voter->deferred_trx_id) {
                  //XXX cancel_deferred_transaction(voter->deferred_trx_id);
               }
               voters_tbl.modify( voter, 0, [&](voter_info& a) {
                     a.staked += a.unstaking;
                     a.unstaking.quantity = 0;
                     a.unstake_per_week.quantity = 0;
                     a.deferred_trx_id = 0;
                     a.last_update = now();
                  });
            }
         }

         static system_token_type payment_per_block(uint32_t percent_of_max_inflation_rate) {
            const system_token_type token_supply = currency::get_total_supply();
            const double annual_rate = double(max_inflation_rate * percent_of_max_inflation_rate) / double(10000);
            double continuous_rate = std::log1p(annual_rate);
            uint64_t payment = static_cast<uint64_t>((continuous_rate * double(token_supply.quantity)) / double(blocks_per_year));
            return (system_token_type(payment));
         }




         static void update_elected_producers(time cycle_time) {
        	 
           producers_table producers_tbl( SystemAccount, SystemAccount );
           auto idx = producers_tbl.template get_index<N(prototalvote)>();

           uint32_t *base_per_transaction_net_usage = new uint32_t[31];
           uint32_t *base_per_transaction_cpu_usage = new uint32_t[31];
           uint32_t *base_per_action_cpu_usage = new uint32_t[31];
           uint32_t *base_setcode_cpu_usage = new uint32_t[31];
           uint32_t *per_signature_cpu_usage = new uint32_t[31];
           uint32_t *per_lock_net_usage = new uint32_t[31];
           uint64_t *context_free_discount_cpu_usage_num = new uint64_t[31];
           uint64_t *context_free_discount_cpu_usage_den = new uint64_t[31];
           uint32_t *max_transaction_cpu_usage = new uint32_t[31];
           uint32_t *max_transaction_net_usage = new uint32_t[31];
           uint64_t *max_block_cpu_usage = new uint64_t[31];
           uint32_t *target_block_cpu_usage_pct = new uint32_t[31];
           uint64_t *max_block_net_usage = new uint64_t[31];
           uint32_t *target_block_net_usage_pct = new uint32_t[31];
           uint32_t *max_transaction_lifetime = new uint32_t[31];
           uint16_t *max_authority_depth = new uint16_t[31];
           uint32_t *max_transaction_exec_time = new uint32_t[31];
           uint16_t *max_inline_depth = new uint16_t[31];
           uint32_t *max_inline_action_size = new uint32_t[31];
           uint32_t *max_generated_transaction_count = new uint32_t[31];
           uint32_t *percent_of_max_inflation_rate = new uint32_t[31];
           uint32_t *storage_reserve_ratio = new uint32_t[31];

            aacio::producer_schedule schedule;
            schedule.producers.reserve(31);
            //
            size_t n = 0;
            for ( auto it = idx.crbegin(); it != idx.crend() && n < 31 && 0 < it->total_votes; ++it ) {
            	 
            	if ( it->active() ) {
            		
                  schedule.producers.emplace_back();
                  schedule.producers.back().producer_name = it->owner;
                  aacio_assert( sizeof(schedule.producers.back().block_signing_key) == it->packed_key.size(), "size mismatch" );
                  std::copy( it->packed_key.begin(), it->packed_key.end(), schedule.producers.back().block_signing_key.data );

                  base_per_transaction_net_usage[n] = it->prefs.base_per_transaction_net_usage;
                  base_per_transaction_cpu_usage[n] = it->prefs.base_per_transaction_cpu_usage;
                  base_per_action_cpu_usage[n] = it->prefs.base_per_action_cpu_usage;
                  base_setcode_cpu_usage[n] = it->prefs.base_setcode_cpu_usage;
                  per_signature_cpu_usage[n] = it->prefs.per_signature_cpu_usage;
                  per_lock_net_usage[n] = it->prefs.per_lock_net_usage;
                  context_free_discount_cpu_usage_num[n] = it->prefs.context_free_discount_cpu_usage_num;
                  context_free_discount_cpu_usage_den[n] = it->prefs.context_free_discount_cpu_usage_den;
                  max_transaction_cpu_usage[n] = it->prefs.max_transaction_cpu_usage;
                  max_transaction_net_usage[n] = it->prefs.max_transaction_net_usage;
                  max_block_cpu_usage[n] = it->prefs.max_block_cpu_usage;
                  target_block_cpu_usage_pct[n] = it->prefs.target_block_cpu_usage_pct;
                  max_block_net_usage[n] = it->prefs.max_block_net_usage;
                  target_block_net_usage_pct[n] = it->prefs.target_block_net_usage_pct;
                  max_transaction_lifetime[n] = it->prefs.max_transaction_lifetime;
                  max_authority_depth[n] = it->prefs.max_authority_depth;
                  max_transaction_exec_time[n] = it->prefs.max_transaction_exec_time;
                  max_inline_depth[n] = it->prefs.max_inline_depth;
                  max_inline_action_size[n] = it->prefs.max_inline_action_size;
                  max_generated_transaction_count[n] = it->prefs.max_generated_transaction_count;

                  storage_reserve_ratio[n] = it->prefs.storage_reserve_ratio;
                  percent_of_max_inflation_rate[n] = it->prefs.percent_of_max_inflation_rate;
                  ++n;
               }
            }

            if ( n == 0 ) { //no active producers with votes > 0
               return;
            }
            if ( 1 < n ) {
            	
               std::sort( base_per_transaction_net_usage, base_per_transaction_net_usage+n );
               std::sort( base_per_transaction_cpu_usage, base_per_transaction_cpu_usage+n );
               std::sort( base_per_action_cpu_usage, base_per_action_cpu_usage+n );
               std::sort( base_setcode_cpu_usage, base_setcode_cpu_usage+n );
               std::sort( per_signature_cpu_usage, per_signature_cpu_usage+n );
               std::sort( per_lock_net_usage, per_lock_net_usage+n );
               std::sort( context_free_discount_cpu_usage_num, context_free_discount_cpu_usage_num+n );
               std::sort( context_free_discount_cpu_usage_den, context_free_discount_cpu_usage_den+n );
               std::sort( max_transaction_cpu_usage, max_transaction_cpu_usage+n );
               std::sort( max_transaction_net_usage, max_transaction_net_usage+n );
               std::sort( max_block_cpu_usage, max_block_cpu_usage+n );
               std::sort( target_block_cpu_usage_pct, target_block_cpu_usage_pct+n );
               std::sort( max_block_net_usage, max_block_net_usage+n );
               std::sort( target_block_net_usage_pct, target_block_net_usage_pct+n );
               std::sort( max_transaction_lifetime, max_transaction_lifetime+n );
               std::sort( max_transaction_exec_time, max_transaction_exec_time+n );
               std::sort( max_authority_depth, max_authority_depth+n );
               std::sort( max_inline_depth, max_inline_depth+n );
               std::sort( max_inline_action_size, max_inline_action_size+n );
               std::sort( max_generated_transaction_count, max_generated_transaction_count+n );
               std::sort( storage_reserve_ratio, storage_reserve_ratio+n );
               std::sort( percent_of_max_inflation_rate, percent_of_max_inflation_rate+n );

            }

            // should use producer_schedule_type from libraries/chain/include/aacio/chain/producer_schedule.hpp
            bytes packed_schedule = pack(schedule);
            set_active_producers( packed_schedule.data(),  packed_schedule.size() );
            size_t median = n/2;

            auto parameters = global_state_singleton::exists() ? global_state_singleton::get()
                  : common<SystemAccount>::get_default_parameters();

            parameters.base_per_transaction_net_usage = base_per_transaction_net_usage[median];
            parameters.base_per_transaction_cpu_usage = base_per_transaction_cpu_usage[median];
            parameters.base_per_action_cpu_usage = base_per_action_cpu_usage[median];
            parameters.base_setcode_cpu_usage = base_setcode_cpu_usage[median];
            parameters.per_signature_cpu_usage = per_signature_cpu_usage[median];
            parameters.per_lock_net_usage = per_lock_net_usage[median];
            parameters.context_free_discount_cpu_usage_num = context_free_discount_cpu_usage_num[median];
            parameters.context_free_discount_cpu_usage_den = context_free_discount_cpu_usage_den[median];
            parameters.max_transaction_cpu_usage = max_transaction_cpu_usage[median];
            parameters.max_transaction_net_usage = max_transaction_net_usage[median];
            parameters.max_block_cpu_usage = max_block_cpu_usage[median];
            parameters.target_block_cpu_usage_pct = target_block_cpu_usage_pct[median];
            parameters.max_block_net_usage = max_block_net_usage[median];
            parameters.target_block_net_usage_pct = target_block_net_usage_pct[median];
            parameters.max_transaction_lifetime = max_transaction_lifetime[median];
            parameters.max_transaction_exec_time = max_transaction_exec_time[median];
            parameters.max_authority_depth = max_authority_depth[median];
            parameters.max_inline_depth = max_inline_depth[median];
            parameters.max_inline_action_size = max_inline_action_size[median];
            parameters.max_generated_transaction_count = max_generated_transaction_count[median];
            parameters.storage_reserve_ratio = storage_reserve_ratio[median];
            parameters.percent_of_max_inflation_rate = percent_of_max_inflation_rate[median];

            // not voted on
            parameters.first_block_time_in_cycle = cycle_time;

            // derived parameters
            auto half_of_percentage = parameters.percent_of_max_inflation_rate / 2;
            auto other_half_of_percentage = parameters.percent_of_max_inflation_rate - half_of_percentage;
            parameters.payment_per_block = payment_per_block(half_of_percentage);
            parameters.payment_to_aac_bucket = payment_per_block(other_half_of_percentage);
            parameters.blocks_per_cycle = common<SystemAccount>::blocks_per_producer * schedule.producers.size();

            if ( parameters.max_storage_size < parameters.total_storage_bytes_reserved ) {
               parameters.max_storage_size = parameters.total_storage_bytes_reserved;
            }

            auto issue_quantity = parameters.blocks_per_cycle * (parameters.payment_per_block + parameters.payment_to_aac_bucket);
            currency::inline_issue(SystemAccount, issue_quantity);
            set_blockchain_parameters(parameters);
            global_state_singleton::set(parameters);

            delete []base_per_transaction_net_usage;
            delete []base_per_transaction_cpu_usage;
            delete []base_per_action_cpu_usage;
            delete []base_setcode_cpu_usage;
            delete []per_signature_cpu_usage;
            delete []per_lock_net_usage;
            delete []context_free_discount_cpu_usage_num;
            delete []context_free_discount_cpu_usage_den;
            delete []max_transaction_cpu_usage;
            delete []max_transaction_net_usage;
            delete []max_block_cpu_usage;
            delete []target_block_cpu_usage_pct;
            delete []max_block_net_usage;
            delete []target_block_net_usage_pct;
            delete []max_transaction_lifetime;
            delete []max_authority_depth;
            delete []max_transaction_exec_time;
            delete []max_inline_depth;
            delete []max_inline_action_size;
            delete []max_generated_transaction_count;
            delete []percent_of_max_inflation_rate;
            delete []storage_reserve_ratio;

         }

         ACTION( SystemAccount, voteproducer ) {
            account_name                voter;
            account_name                proxy;
            std::vector<account_name>   producers;

            AACLIB_SERIALIZE( voteproducer, (voter)(proxy)(producers) )
         };

         /**
          *  @pre vp.producers must be sorted from lowest to highest
          *  @pre if proxy is set then no producers can be voted for
          *  @pre every listed producer or proxy must have been previously registered
          *  @pre vp.voter must authorize this action
          *  @pre voter must have previously staked some AAC for voting
          */
         static void on( const voteproducer& vp ) {
            require_auth( vp.voter );

            //validate input
            if ( vp.proxy ) {
               aacio_assert( vp.producers.size() == 0, "cannot vote for producers and proxy at same time" );
               require_recipient( vp.proxy );
            } else {
               aacio_assert( vp.producers.size() <= 30, "attempt to vote for too many producers" );
               for( size_t i = 1; i < vp.producers.size(); ++i ) {
                  aacio_assert( vp.producers[i-1] < vp.producers[i], "producer votes must be unique and sorted" );
               }
            }

            voters_table voters_tbl( SystemAccount, SystemAccount );
            auto voter = voters_tbl.find( vp.voter );

            aacio_assert( voter != voters_tbl.end() && ( 0 < voter->staked.quantity || ( voter->is_proxy && 0 < voter->proxied_votes ) ), "no stake to vote" );
            if ( voter->is_proxy ) {
               aacio_assert( vp.proxy == 0 , "account registered as a proxy is not allowed to use a proxy" );
            }

            //find old producers, update old proxy if needed
            const std::vector<account_name>* old_producers = nullptr;
            if( voter->proxy ) {
               if ( voter->proxy == vp.proxy ) {
                  return; // nothing changed
               }
               auto old_proxy = voters_tbl.find( voter->proxy );
               aacio_assert( old_proxy != voters_tbl.end(), "old proxy not found" ); //data corruption
               voters_tbl.modify( old_proxy, 0, [&](auto& a) { a.proxied_votes -= voter->staked.quantity; } );
               if ( old_proxy->is_proxy ) { //if proxy stoped being proxy, the votes were already taken back from producers by on( const unregister_proxy& )
                  old_producers = &old_proxy->producers;
               }
            } else {
               old_producers = &voter->producers;
            }

            //find new producers, update new proxy if needed
            const std::vector<account_name>* new_producers = nullptr;
            if ( vp.proxy ) {
               auto new_proxy = voters_tbl.find( vp.proxy );
               aacio_assert( new_proxy != voters_tbl.end() && new_proxy->is_proxy, "proxy not found" );
               voters_tbl.modify( new_proxy, 0, [&](auto& a) { a.proxied_votes += voter->staked.quantity; } );
               new_producers = &new_proxy->producers;
            } else {
               new_producers = &vp.producers;
            }

            producers_table producers_tbl( SystemAccount, SystemAccount );
            uint128_t votes = voter->staked.quantity;
            if ( voter->is_proxy ) {
               votes += voter->proxied_votes;
            }

            if ( old_producers ) { //old_producers == nullptr if proxy has stopped being a proxy and votes were taken back from the producers at that moment
               //revoke votes only from no longer elected
               std::vector<account_name> revoked( old_producers->size() );
               auto end_it = std::set_difference( old_producers->begin(), old_producers->end(), new_producers->begin(), new_producers->end(), revoked.begin() );
               for ( auto it = revoked.begin(); it != end_it; ++it ) {
                  auto prod = producers_tbl.find( *it );
                  aacio_assert( prod != producers_tbl.end(), "never existed producer" ); //data corruption
                  producers_tbl.modify( prod, 0, [&]( auto& pi ) { pi.total_votes -= votes; } );
               }
            }

            //update newly elected
            std::vector<account_name> elected( new_producers->size() );
            auto end_it = elected.begin();
            if( old_producers ) {
               end_it = std::set_difference( new_producers->begin(), new_producers->end(), old_producers->begin(), old_producers->end(), elected.begin() );
            } else {
               end_it = std::copy( new_producers->begin(), new_producers->end(), elected.begin() );
            }
            for ( auto it = elected.begin(); it != end_it; ++it ) {
               auto prod = producers_tbl.find( *it );
               aacio_assert( prod != producers_tbl.end(), "producer is not registered" );
               if ( vp.proxy == 0 ) { //direct voting, in case of proxy voting update total_votes even for inactive producers
                  aacio_assert( prod->active(), "producer is not currently registered" );
               }
               producers_tbl.modify( prod, 0, [&]( auto& pi ) { pi.total_votes += votes; } );
            }

            // save new values to the account itself
            voters_tbl.modify( voter, 0, [&](voter_info& a) {
                  a.proxy = vp.proxy;
                  a.last_update = now();
                  a.producers = vp.producers;
               });
         }

         ACTION( SystemAccount, regproxy ) {
            account_name proxy;

            AACLIB_SERIALIZE( regproxy, (proxy) )
         };

         static void on( const regproxy& reg ) {
            require_auth( reg.proxy );

            voters_table voters_tbl( SystemAccount, SystemAccount );
            auto proxy = voters_tbl.find( reg.proxy );
            if ( proxy != voters_tbl.end() ) {
               aacio_assert( proxy->is_proxy == 0, "account is already a proxy" );
               aacio_assert( proxy->proxy == 0, "account that uses a proxy is not allowed to become a proxy" );
               voters_tbl.modify( proxy, 0, [&](voter_info& a) {
                     a.is_proxy = 1;
                     a.last_update = now();
                     //a.proxied_votes may be > 0, if the proxy has been unregistered, so we had to keep the value
                  });
               if ( 0 < proxy->proxied_votes ) {
                  producers_table producers_tbl( SystemAccount, SystemAccount );
                  for ( auto p : proxy->producers ) {
                     auto prod = producers_tbl.find( p );
                     aacio_assert( prod != producers_tbl.end(), "never existed producer" ); //data corruption
                     producers_tbl.modify( prod, 0, [&]( auto& pi ) { pi.total_votes += proxy->proxied_votes; });
                  }
               }
            } else {
               voters_tbl.emplace( reg.proxy, [&]( voter_info& a ) {
                     a.owner = reg.proxy;
                     a.last_update = now();
                     a.proxy = 0;
                     a.is_proxy = 1;
                     a.proxied_votes = 0;
                     a.staked.quantity = 0;
                  });
            }
         }

         ACTION( SystemAccount, unregproxy ) {
            account_name proxy;

            AACLIB_SERIALIZE( unregproxy, (proxy) )
         };

         static void on( const unregproxy& reg ) {
            require_auth( reg.proxy );

            voters_table voters_tbl( SystemAccount, SystemAccount );
            auto proxy = voters_tbl.find( reg.proxy );
            aacio_assert( proxy != voters_tbl.end(), "proxy not found" );
            aacio_assert( proxy->is_proxy == 1, "account is not a proxy" );

            voters_tbl.modify( proxy, 0, [&](voter_info& a) {
                     a.is_proxy = 0;
                     a.last_update = now();
                     //a.proxied_votes should be kept in order to be able to reenable this proxy in the future
               });

            if ( 0 < proxy->proxied_votes ) {
               producers_table producers_tbl( SystemAccount, SystemAccount );
               for ( auto p : proxy->producers ) {
                  auto prod = producers_tbl.find( p );
                  aacio_assert( prod != producers_tbl.end(), "never existed producer" ); //data corruption
                  producers_tbl.modify( prod, 0, [&]( auto& pi ) { pi.total_votes -= proxy->proxied_votes; });
               }
            }
         }

   };
}
