#pragma once
#include <aaciolib/aacio.hpp>

#include <aaciolib/generic_currency.hpp>
#include <aaciolib/multi_index.hpp>
#include <aaciolib/privileged.hpp>
#include <aaciolib/singleton.hpp>

namespace aaciosystem {

   template<account_name SystemAccount>
   class common {
      public:
         static constexpr account_name system_account = SystemAccount;
         typedef aacio::generic_currency< aacio::token<system_account,S(4,AAC)> > currency;
         typedef typename currency::token_type                                    system_token_type;

         static constexpr uint64_t   currency_symbol = currency::symbol;            // S(4,AAC)
         static constexpr uint32_t   max_inflation_rate = 5;                        // 5% annual inflation

         static constexpr uint32_t blocks_per_producer = 6;  //update by ck 2018-5-18
         static constexpr uint32_t   seconds_per_day = 24 * 3600;
         static constexpr uint32_t   days_per_4years = 1461;

         struct aacio_parameters : aacio::blockchain_parameters {
            uint64_t          max_storage_size = 10 * 1024 * 1024;
            uint32_t          percent_of_max_inflation_rate = 0;
            uint32_t          storage_reserve_ratio = 1000;      // ratio * 1000

            AACLIB_SERIALIZE_DERIVED( aacio_parameters, aacio::blockchain_parameters, (percent_of_max_inflation_rate)(storage_reserve_ratio) )
         };

         struct aacio_global_state : aacio_parameters {
            uint64_t             total_storage_bytes_reserved = 0;
            system_token_type    total_storage_stake;
            system_token_type    payment_per_block = system_token_type();
            system_token_type    payment_to_aac_bucket = system_token_type();
            time                 first_block_time_in_cycle = 0;
            uint32_t             blocks_per_cycle = 0;
            time                 last_bucket_fill_time = 0;
            system_token_type    aac_bucket = system_token_type();

            AACLIB_SERIALIZE_DERIVED( aacio_global_state, aacio_parameters, (total_storage_bytes_reserved)(total_storage_stake)
                                      (payment_per_block)(payment_to_aac_bucket)(first_block_time_in_cycle)(blocks_per_cycle)
                                      (last_bucket_fill_time)(aac_bucket) )
         };

         typedef aacio::singleton<SystemAccount, N(inflation), SystemAccount, aacio_global_state> global_state_singleton;

         static aacio_global_state& get_default_parameters() {
            static aacio_global_state dp;
            get_blockchain_parameters(dp);
            return dp;
         }
   };

}
