/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */
#pragma once

#include <aaciolib/asset.hpp>
#include <aaciolib/aacio.hpp>
#include <aaciolib/singleton.hpp>

#include <string>

namespace aacio {

   class betstar : public contract {
      public:
         betstar( account_name self ):contract(self), configtable(_self,_self) {
            if ( configtable.exists() )
               cfg = configtable.get();
         }

         void create();

         void bet( account_name participant, uint32_t star, asset quantity );

         void lot();

         void setfee( uint32_t fee );

         void setjrate( uint32_t rate );

      private:
         static constexpr double p0 = 0.1500;
         static constexpr double p1 = 0.2920;
         static constexpr double p2 = 0.2482;
         static constexpr double p3 = 0.1763;
         static constexpr double p4 = 1 - p0 - p1 - p2 - p3;
         static constexpr uint32_t base0 = 0;
         static constexpr uint32_t base1 = 0xffffffff / 2;
         static constexpr uint32_t upper00 = base0   + static_cast<uint32_t>(base1 * p0);
         static constexpr uint32_t upper01 = upper00 + static_cast<uint32_t>(base1 * p1);
         static constexpr uint32_t upper02 = upper01 + static_cast<uint32_t>(base1 * p2);
         static constexpr uint32_t upper03 = upper02 + static_cast<uint32_t>(base1 * p3);
         static constexpr uint32_t upper10 = base1   + static_cast<uint32_t>(base1 * p0);
         static constexpr uint32_t upper11 = upper10 + static_cast<uint32_t>(base1 * p1);
         static constexpr uint32_t upper12 = upper11 + static_cast<uint32_t>(base1 * p2);
         static constexpr uint32_t upper13 = upper12 + static_cast<uint32_t>(base1 * p3);

         struct part_info {
            account_name  participant;
            uint32_t      star;
            asset         quantity;

            part_info() {}
            part_info(account_name p, uint32_t s, asset q) : participant(p), star(s), quantity(q) {}
            uint64_t primary_key()const { return participant; }

            AACLIB_SERIALIZE( part_info, (participant)(star)(quantity) )
         };

         struct issue {
            uint64_t id;
            uint32_t participant_num;
            asset    sum;
            uint32_t result;
            bool     is_ongoing = true;
            std::vector<part_info> participants;

            uint64_t primary_key()const { return id; }
         };

         struct multi_winner {
            account_name  participant;
            uint32_t      count;

            uint64_t primary_key()const { return participant; }

            AACLIB_SERIALIZE( multi_winner, (participant)(count) )
         };

         struct term {
            uint64_t id;
            std::vector<std::vector<account_name>> winners = std::vector<std::vector<account_name>>(10);

            uint64_t primary_key()const { return id; }
         };

         struct bet_config {
            uint32_t  fee = 500;  /// unit: 1/10000
            uint32_t  jackpot_rate = 8000;  /// unit: 1/10000
            uint64_t  current_id = 0;
            std::vector<uint64_t> results = std::vector<uint64_t>(5);
         };

         typedef aacio::multi_index<N(issue), issue> issues;
         typedef aacio::multi_index<N(term), term> terms;
         typedef aacio::singleton<N(config), bet_config> config;

         uint32_t get_random();

         uint32_t determine_star(uint32_t random) {
             if ( (random < upper00) || ((random >= base1) && (random < upper10)) ) {
                 return 0;
             }
             else if ( ((random >= upper00) && (random < upper01)) || ((random >= upper10) && (random < upper11)) ) {
                 return 1;
             }
             else if ( ((random >= upper01) && (random < upper02)) || ((random >= upper11) && (random < upper12)) ) {
                 return 2;
             }
             else if ( ((random >= upper02) && (random < upper03)) || ((random >= upper12) && (random < upper13)) ) {
                 return 3;
             }
             else {
                 return 4;
             }
         }

         void update_multi_winners(uint64_t term_id, uint64_t issue_id, const std::vector<account_name> &winners);
         void award_multi_winners(uint64_t term_id, uint64_t issue_id);
         asset get_jackpot_balance();

         // taken from aacio.token.hpp
         struct account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.name(); }
         };

         // taken from aacio.token.hpp
         typedef aacio::multi_index<N(accounts), account> accounts;

         config configtable;
         bet_config cfg;
   };

} /// namespace aacio
