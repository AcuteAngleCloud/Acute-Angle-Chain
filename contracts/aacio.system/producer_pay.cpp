#include "aacio.system.hpp"

#include <aacio.token/aacio.token.hpp>

namespace aaciosystem {

   const int64_t  min_activated_stake   = 150'000'000'0000; // 5%
   const double   continuous_rate       = 0.04879;          // 5% annual rate
   const double   reward_rate           = 0.60;
   const double   reward_perblock_rate  = 0.20;             // 25%
   const double   reward_standby_rate   = 0.40;             // 50%
   const double   reward_user_vote_rate = 0.40;             // 25%
   const uint32_t blocks_per_year       = 52*7*24*3600;   // half seconds per year
   const uint32_t seconds_per_year      = 52*7*24*3600;
   const uint32_t blocks_per_day        = 24 * 3600;
   const uint32_t blocks_per_hour       = 3600;
   const uint32_t blocks_per_minute     = 60;
   const uint64_t useconds_per_day      = 24 * 3600 * uint64_t(1000000);
   const uint64_t useconds_per_year     = seconds_per_year*1000000ll;
   const uint64_t fill_bucket_frequency = blocks_per_minute*1000000ll; /** fill bucket scheduled task frequency(useconds) */


   void system_contract::onblock( block_timestamp timestamp, account_name producer ) {
      using namespace aacio;

      require_auth(N(aacio));

      /** until activated stake crosses this threshold no new rewards are paid */
      if( _gstate.total_activated_stake < min_activated_stake )
         return;

      if( _gstate.last_pervote_bucket_fill == 0 )  /// start the presses
         _gstate.last_pervote_bucket_fill = current_time();


      /**
       * At startup the initial producer may not be one that is registered / elected
       * and therefore there may be no producer object for them.
       */
      auto prod = _producers.find(producer);
      if ( prod != _producers.end() ) {
         _gstate.total_unpaid_blocks++;
         _producers.modify( prod, 0, [&](auto& p ) {
               p.unpaid_blocks++;
         });
      }

      /// only update block producers once every minute, block_timestamp is in half seconds
      if( timestamp.slot - _gstate.last_producer_schedule_update.slot > 120 ) {
            maybe_start_bid_chain();
            print("before [fill_bucket_schedule()1]\n");
            
            fill_bucket_schedule();

            update_elected_producers(timestamp);

            if ((timestamp.slot - _gstate.last_name_close.slot) > blocks_per_day) {
                  name_bid_table bids(_self, _self);
                  auto idx = bids.get_index<N(highbid)>();
                  auto highest = idx.begin();
                  if (highest != idx.end() &&
                      highest->high_bid > 0 &&
                      highest->last_bid_time < (current_time() - useconds_per_day) &&
                      _gstate.thresh_activated_stake_time > 0 &&
                      (current_time() - _gstate.thresh_activated_stake_time) > 14 * useconds_per_day) {
                        _gstate.last_name_close = timestamp;
                        idx.modify(highest, 0, [&](auto &b) {
                              b.high_bid = -b.high_bid;
                        });
                  }
         }
      }
   }

   using namespace aacio;
   void system_contract::fill_bucket_schedule( ) {
         print("[fill_bucket_schedule2] begin\n");
         
             
             aacio_assert(_gstate.total_activated_stake >= min_activated_stake,
                          "cannot claim rewards until the chain is activated (at least 15% of all tokens participate in voting)");
        
         auto ct = current_time();
        
         const auto usecs_since_last_fill = ct - _gstate.last_pervote_bucket_fill;
         print("usecs_since_last_fill is:", usecs_since_last_fill);
         print("\n");

         
         if (usecs_since_last_fill >= fill_bucket_frequency && _gstate.last_pervote_bucket_fill > 0) {
              
               const asset token_supply = token(N(aacio.token)).get_supply(symbol_type(system_token_symbol).name());
               
               auto new_tokens = static_cast<int64_t>((continuous_rate * double(token_supply.amount) * double(usecs_since_last_fill)) / double(useconds_per_year));
               
               auto to_rewards = static_cast<int64_t>(new_tokens * reward_rate);
               
               auto to_savings = static_cast<int64_t>(new_tokens - to_rewards);
               
               auto to_per_block_pay = static_cast<int64_t>(to_rewards * reward_perblock_rate);
               
               auto to_per_vote_pay = static_cast<int64_t>(to_rewards * reward_standby_rate);
               
               auto to_user_vote_pay = static_cast<int64_t>(to_rewards * reward_user_vote_rate);
               
               INLINE_ACTION_SENDER(aacio::token, issue)( N(aacio.token), {{N(aacio), N(active)}},
                {N(aacio), asset(new_tokens), std::string("issue tokens for producer pay and savings")});

               INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio), N(active)},
                {N(aacio), N(aacio.saving), asset(to_savings), "unallocated inflation"}); //未分配的通货膨胀

               INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio), N(active)},
                {N(aacio), N(aacio.bpay), asset(to_per_block_pay), "fund per-block bucket"});

               INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio), N(active)},
                {N(aacio), N(aacio.vpay), asset(to_per_vote_pay), "fund per-vote bucket"});
               INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio), N(active)},
                {N(aacio), N(aacio.uvpay), asset(to_user_vote_pay), "fund user-vote bucket"});

               _gstate.pervote_bucket += to_per_vote_pay;
               
               _gstate.perblock_bucket += to_per_block_pay;
              
               _gstate.peruser_vote_bucket += to_user_vote_pay;
               
               _gstate.last_pervote_bucket_fill = ct;
               
               int64_t pervote_bucket_used = 0;
               int64_t perblock_bucket_used = 0;
               int64_t total_unpaid_blocks_used = 0;
               int64_t total_votes_used = 0;

               /** Record award division for producers */

               for (auto & prod : _producers) {

                     if (false == prod.active()) {
                           continue;
                     }
                     
                     int64_t producer_per_block_pay = 0;
                     if (_gstate.total_unpaid_blocks > 0) {
                           producer_per_block_pay = (_gstate.perblock_bucket * prod.unpaid_blocks) / _gstate.total_unpaid_blocks;
                     }
                     int64_t producer_per_vote_pay = 0;
                     if (_gstate.total_producer_vote_weight > 0) {
                           producer_per_vote_pay = int64_t((_gstate.pervote_bucket * prod.total_votes) / _gstate.total_producer_vote_weight);
                     }
                     pervote_bucket_used += producer_per_vote_pay;
                   
                     perblock_bucket_used += producer_per_block_pay;
                    
                     total_unpaid_blocks_used += prod.unpaid_blocks;
                    

                     total_votes_used += prod.total_votes;
                     
                     #if 1
                     _producers.modify(prod, 0, [&](auto &p) {
                           p.unpaid_blocks = 0;
                           p.rewards_block_balance += producer_per_block_pay;
                          
                           p.rewards_vote_balance += producer_per_vote_pay;
                          
                     });
                     #endif
                     
               }

               _gstate.pervote_bucket -= pervote_bucket_used; 
               _gstate.perblock_bucket -= perblock_bucket_used;
               _gstate.total_unpaid_blocks -= total_unpaid_blocks_used;
              

               /** Record award division for votes */
               if (_gstate.total_activated_stake > 0) {
                     int64_t peruser_vote_bucket_used = 0;
                     for (auto & vote : _voters) {
                          
                           int64_t voter_per_vote_pay = 0;
                           if(vote.last_vote_weight <= 0.0){
                              print("user is not voter for anyone\n" );
                              continue;
                           }

                           voter_per_vote_pay = int64_t((_gstate.peruser_vote_bucket * vote.staked) / _gstate.total_activated_stake);

                           peruser_vote_bucket_used += voter_per_vote_pay;
                           
                           _voters.modify(vote, 0, [&](auto &v) {
                                 v.rewards_vote_balance += voter_per_vote_pay;
                                 
                           });
                     }
                     
                     _gstate.peruser_vote_bucket -= peruser_vote_bucket_used;
                    
               }
         }

   }
   void system_contract::claimrewards( const account_name& owner ) {
      require_auth(owner);

      aacio_assert( _gstate.total_activated_stake >= min_activated_stake,
                    "cannot claim rewards until the chain is activated (at least 15% of all tokens participate in voting)" );

      auto ct = current_time();

      auto prod = _producers.find( owner );
      if( prod != _producers.end() ) {
         aacio_assert( prod->active(), "producer does not have an active key" );
         aacio_assert( ct - prod->last_claim_time > fill_bucket_frequency, "already claimed rewards within past frequency" );

         if( prod->rewards_block_balance > 0 ) {
            INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.bpay),N(active)},
                                                         { N(aacio.bpay), owner, asset(prod->rewards_block_balance), std::string("producer get block claim") } );
         }
         if( prod->rewards_vote_balance > 0 ) {
            INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.vpay),N(active)},
                                                         { N(aacio.vpay), owner, asset(prod->rewards_vote_balance), std::string("producer get vote claim") } );
         }
         
         _producers.modify( prod, 0, [&](auto& p) {
            p.last_claim_time = ct;
            p.rewards_block_balance = 0;
            p.rewards_vote_balance = 0;
         });
	  
      }
      else{
         auto voter = _voters.find( owner );
         aacio_assert( voter != _voters.end(), "invalid user specified" ); 
         aacio_assert( ct - voter->last_claim_time > fill_bucket_frequency, "already claimed rewards within past frequency" );
         if( voter->rewards_vote_balance > 0 ) {
            INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.uvpay),N(active)},
                                                         { N(aacio.uvpay), owner, asset(voter->rewards_vote_balance), std::string("voter get vote clam") } );
         }

         _voters.modify( voter, 0, [&](auto& v) {
            v.last_claim_time = ct;
            v.rewards_vote_balance = 0;
         });
      }
   }

} //namespace aaciosystem
