/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */

#include <algorithm>
#include <aaciolib/crypto.h>
#include <aaciolib/transaction.h>
#include <aacio.token/aacio.token.hpp>
#include "betstar.hpp"

namespace aacio {

void betstar::create()
{
   require_auth( N(aacio.bspool) );

   if (cfg.current_id > 0) {
      issues lastissuestable( _self, cfg.current_id );
      auto lastissue = lastissuestable.find( cfg.current_id );
      aacio_assert( (lastissue != lastissuestable.end()) && (!lastissue->is_ongoing), "the last issue has not been closed" );
   }  

   cfg.current_id += 1;
   issues issuestable( _self, cfg.current_id );
   auto issue = issuestable.find( cfg.current_id );
   aacio_assert( issue == issuestable.end(), "the issue already exists" );

   if ( (cfg.current_id % 10) == 1 ) {
      terms termstable( _self, cfg.current_id );
      auto term = termstable.find( cfg.current_id );
      aacio_assert( term == termstable.end(), "the term already exists" );
   }

   issuestable.emplace( _self, [&]( auto& i ) {
      i.id              = cfg.current_id;
      i.participant_num = 0;
      i.sum             = asset(0);
      i.result          = 0;
      i.is_ongoing      = true;
   });

   if ( (cfg.current_id % 10) == 1 ) {
      terms termstable( _self, cfg.current_id );
      termstable.emplace( _self, [&]( auto& t ) {
          t.id          = cfg.current_id;
      });
   }
   configtable.set( cfg, _self );
}

void betstar::bet( account_name participant, uint32_t star, asset quantity )
{
   require_auth( participant );
   aacio_assert( star < 5, "invalid star");
   aacio_assert( quantity.symbol == asset().symbol, "asset must be system token" );
   aacio_assert( (quantity.amount >= 1000000) && (quantity.amount <= 100000000), "must between 100...10000 tokens" );

   issues issuestable( _self, cfg.current_id );
   auto issue = issuestable.find( cfg.current_id );
   aacio_assert( (issue != issuestable.end()) && issue->is_ongoing, "the issue is not available" );
   auto iparticipant = find_if(issue->participants.begin(), issue->participants.end(),
                    [&] (const part_info& p) { return p.participant == participant; });
   aacio_assert( iparticipant == issue->participants.end(), "have already betted" );

   INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {participant,N(active)}, { participant, N(aacio.bspool), quantity, std::string("betstar") } );
   issuestable.modify( issue, 0, [&]( auto& i ) {
         i.participant_num++;
         i.sum += quantity;
         i.participants.emplace_back(participant, star, quantity);
      });
}

void betstar::lot()
{
   require_auth( N(aacio.bspool) );

   issues issuestable( _self, cfg.current_id );
   auto issue = issuestable.find( cfg.current_id );
   aacio_assert( (issue != issuestable.end()) && issue->is_ongoing, "the issue is not available" );

   uint32_t star = determine_star( get_random() );
   issuestable.modify( issue, 0, [&]( auto& i ) {
         i.result     = star;
         i.is_ongoing = false;
      });

   ++cfg.results[star];
   configtable.set( cfg, _self );

   std::vector<account_name> winners;
   for (auto p : issue->participants) {
      if (p.star == star) {
         winners.push_back(p.participant);
         asset award = p.quantity;
         award.amount *= (star + 2);
         award.amount += (p.quantity.amount * cfg.bonus_rate / 10);
         asset fee = p.quantity;
         fee.amount = award.amount * cfg.fee / 10000;
         award -= fee;
         INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.bspool),N(active)}, { N(aacio.bspool), p.participant, award, std::string("betstar award") } );
         INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.bspool),N(active)}, { N(aacio.bspool), N(betstar.fee), fee, std::string("betstar fee") } );
      }
   }
   if (!winners.empty()) {
      std::sort(winners.begin(), winners.end());
   }
   uint64_t term_id = (cfg.current_id - 1) / 10 * 10 + 1;
   update_multi_winners(term_id, cfg.current_id, winners);
   award_multi_winners(term_id, cfg.current_id);
}

void betstar::setfee( uint32_t fee )
{
   require_auth( N(aacio.config) );

   aacio_assert( fee <= 10000, "must between 0...10000" );
   cfg.fee = fee;
   configtable.set( cfg, _self );
}

void betstar::setjrate( uint32_t rate )
{
   require_auth( N(aacio.config) );

   aacio_assert( rate <= 10000, "must between 0...10000" );
   cfg.jackpot_rate = rate;
   configtable.set( cfg, _self );
}

void betstar::setbrate( uint32_t rate )
{
   require_auth( N(aacio.config) );

   aacio_assert( rate <= 20, "must between 0...20" );
   cfg.bonus_rate = rate;
   configtable.set( cfg, _self );
}

void betstar::clear( uint64_t id )
{
   issues issuestable( _self, id );
   auto issue = issuestable.find( id );
   aacio_assert( issue != issuestable.end(), "the issue is not available" );
   issuestable.erase( issue );

   if ( (id % 10) == 1 ) {
      terms termstable( _self, id );
      auto term = termstable.find( id );
      aacio_assert( term != termstable.end(), "the term is not available" );
      termstable.erase( term );
   }
}

uint32_t betstar::get_random()
{
   char buf[2048];
   memset(buf, 0, 2048);
   read_transaction(buf, 2048);
   checksum256 hash;
   sha256( buf, 2048, &hash );
   uint32_t number = 0;
   memcpy(&number, hash.hash, 4);
   return number;
}

void betstar::update_multi_winners(uint64_t term_id, uint64_t issue_id, const std::vector<account_name> &winners)
{
   terms termstable( _self, term_id );
   auto term = termstable.find( term_id );
   aacio_assert( term != termstable.end(), "the term is not available" );

   termstable.modify( term, 0, [&]( auto& t ) {
         std::vector<std::vector<account_name>>::size_type count = static_cast<std::vector<std::vector<account_name>>::size_type>(issue_id - term_id);
         if (count == 0) {
            t.winners[0] = winners;
         }
         else {
            std::vector<std::vector<account_name>>::size_type i;
            for (i = count; i != 0; --i) {
               t.winners[i].resize(std::min(t.winners[i-1].size(), winners.size()));
               auto iter = set_intersection(t.winners[i-1].begin(), t.winners[i-1].end(),
                                            winners.begin(), winners.end(), t.winners[i].begin());
               t.winners[i].resize(static_cast<std::vector<account_name>::size_type>(iter - t.winners[i].begin()));
            }
            std::vector<account_name> diff = winners;
            std::vector<account_name> tmp;
            for (i = count; i != 0; --i) {
               set_difference(diff.begin(), diff.end(),
                              t.winners[i].begin(), t.winners[i].end(),
                              std::inserter(tmp, tmp.begin()));
               diff = tmp;
               tmp.resize(0);
            }
            t.winners[0] = diff;
         }
      });
}

void betstar::award_multi_winners(uint64_t term_id, uint64_t issue_id)
{
   std::vector<std::vector<account_name>>::size_type count = static_cast<std::vector<std::vector<account_name>>::size_type>(issue_id - term_id);
   if (count == 0) {
      return;
   }

   terms termstable( _self, term_id );
   auto term = termstable.find( term_id );
   aacio_assert( term != termstable.end(), "the term is not available" );

   issues issuestable( _self, issue_id );
   auto issue = issuestable.find( issue_id );
   aacio_assert( issue != issuestable.end(), "the issue is not available" );

   vector<account_name>::size_type size;
   if (count == 9) {
      size = term->winners[count].size();
      if (size > 0) {
         asset award = get_jackpot_balance();
         award.amount = award.amount * cfg.jackpot_rate / 10000 / size;
         asset fee = award;
         fee.amount = award.amount * cfg.fee / 10000;
         award -= fee;
         for (auto w : term->winners[count]) {
            INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.bspool),N(active)}, { N(aacio.bspool), w, award, std::string("top award") } );
            INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.bspool),N(active)}, { N(aacio.bspool), N(betstar.fee), fee, std::string("betstar fee") } );
         }
      }
      --count;
   }

   for (std::vector<std::vector<account_name>>::size_type i = count; i != 0; --i) {
      size = term->winners[i].size();
      if (size > 0) {
         asset award = issue->sum;
         award.amount = award.amount * (i + 3) / 100 / size;
         asset fee = award;
         fee.amount = award.amount * cfg.fee / 10000;
         award -= fee;
         for (auto w : term->winners[i]) {
            INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.bspool),N(active)}, { N(aacio.bspool), w, award, std::to_string(i + 1) + " wins award" } );
            INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.bspool),N(active)}, { N(aacio.bspool), N(betstar.fee), fee, std::string("betstar fee") } );
         }
      }
   }
}

asset betstar::get_jackpot_balance()
{
   accounts accountstable( N(aacio.token), N(aacio.bspool) );

   auto jackpot = accountstable.find( asset(0).symbol.name() );
   aacio_assert( jackpot != accountstable.end(), "the account does not exist" );

   return jackpot->balance;
}

} /// namespace aacio

AACIO_ABI( aacio::betstar, (create)(bet)(lot)(setfee)(setjrate)(setbrate)(clear) )
