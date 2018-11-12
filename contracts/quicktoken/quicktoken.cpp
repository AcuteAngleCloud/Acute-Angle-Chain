/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */

#include <aaciolib/asset.hpp>
#include <aaciolib/aacio.hpp>
#include <aaciolib/action.hpp>
#include <aaciolib/transaction.hpp>
#include <aaciolib/crypto.h>
#include <aaciolib/types.hpp>
#include <aacio.token/aacio.token.hpp>

#include <string>

using std::string;
using std::vector;
using aacio::contract;
using aacio::action;
using aacio::asset;
using aacio::print;
using aacio::name;
using aacio::symbol_type;
using aacio::symbol_name;
using aacio::string_to_symbol;
using aacio::is_valid_symbol;

namespace quicktoken {

    class quicktoken : public contract {
        public:
            quicktoken( account_name self )
            :contract(self),
            _issue_config(_self,_self)
            {}

            // @abi action
            void create(asset  maximum_supply)
            {
                require_auth( _self );

                auto sym = maximum_supply.symbol;
                aacio_assert( sym.is_valid(), "invalid symbol name" );
                aacio_assert( maximum_supply.is_valid(), "invalid supply");
                aacio_assert( maximum_supply.amount > 0, "max-supply must be positive");

                stats statstable( _self, sym.name() );
                auto existing = statstable.find( sym.name() );
                aacio_assert( existing == statstable.end(), "quick token with symbol already exists" );
                
                ///检测发币指标
                auto isscfg = _issue_config.find( maximum_supply.symbol.name() );
                aacio_assert( isscfg != _issue_config.end(), "issuance of indicators does not exist" );
                aacio_assert( true == isscfg->valid_flag, "issuance of indicators have already used" );

                _issue_config.modify( isscfg, 0, [&]( auto& i ){
                    i.create_time = current_time();
                    i.valid_flag = false;
                });

                statstable.emplace( _self, [&]( auto& s ) {
                    s.supply.symbol = maximum_supply.symbol;
                    s.max_supply    = maximum_supply;
                    s.issuer        = _self;
                });
            }

            // @abi action
            void issue( account_name to, asset quantity, string memo )
            {
                auto sym = quantity.symbol;
                aacio_assert( sym.is_valid(), "invalid symbol name" );
                aacio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

                auto sym_name = sym.name();
                stats statstable( _self, sym_name );
                auto existing = statstable.find( sym_name );
                aacio_assert( existing != statstable.end(), "quick token with symbol does not exist, create token before issue" );
                const auto& st = *existing;

                require_auth( st.issuer );
                aacio_assert( quantity.is_valid(), "invalid quantity" );
                aacio_assert( quantity.amount > 0, "must issue positive quantity" );

                aacio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
                aacio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

                statstable.modify( st, 0, [&]( auto& s ) {
                    s.supply += quantity;
                });

                add_balance( st.issuer, quantity );

                if( to != st.issuer ) {
                    SEND_INLINE_ACTION( *this, transfer, {st.issuer,N(active)}, {st.issuer, to, quantity, memo} );
                }
            }

            // @abi action
            void transfer( account_name from,
                                account_name to,
                                asset        quantity,
                                string       memo )
            {
                aacio_assert( from != to, "cannot transfer to self" );
                require_auth( from );

                aacio_assert( is_account( to ), "to account does not exist");
                auto sym = quantity.symbol.name();
                stats statstable( _self, sym );
                const auto& st = statstable.get( sym );
                
                require_recipient( from );
                require_recipient( to );

                aacio_assert( quantity.is_valid(), "invalid quantity" );
                aacio_assert( quantity.amount > 0, "must transfer positive quantity" );
                aacio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
                aacio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

                sub_balance( from, quantity );
                add_balance( to, quantity );
            }

            // @abi action
            void setissuecfg( string symbol_name)
            {
                require_auth( N(aacio.config) );
                
                aacio_assert( true == is_valid_symbol(string_to_symbol(0, symbol_name.c_str())) && symbol_name.length() <= 7, "symbol name is not valid" );
                
				auto isscfg = _issue_config.find( string_to_symbol(0, symbol_name.c_str())>>8 );
                aacio_assert( isscfg == _issue_config.end(), "quick token with symbol already exists" );

                _issue_config.emplace( _self, [&]( auto& i ){
                    i.symbol_name    = string_to_symbol(0, symbol_name.c_str())>>8;
                    i.authorization_time   = current_time();
                });
            }

        private:
        
            // @abi table accounts i64
            struct account {
                asset    balance;                              
                uint64_t primary_key()const { return balance.symbol.name(); }
            };

            typedef aacio::multi_index<N(accounts), account> accounts;

            // @abi table stat i64
            struct currency_stat {
                asset             supply;
                asset             max_supply;
                account_name      issuer;

                uint64_t primary_key()const { return supply.symbol.name(); }
            };

            typedef aacio::multi_index<N(stat), currency_stat> stats;
            
            // @abi table issuecfg i64
            struct issue_config {
                uint64_t          symbol_name;
                uint64_t          authorization_time = 0;
                uint64_t          create_time = 0;
                bool              valid_flag = true;

                uint64_t primary_key()const { return symbol_name; }
            };
            typedef aacio::multi_index<N(issuecfg), issue_config> issuecfg_tables;

            /// 发币指标信息
            issuecfg_tables _issue_config;

            void sub_balance( account_name owner, asset value ) {
                
                accounts from_acnts( _self, owner );

                const auto& from = from_acnts.get( value.symbol.name(), "no balance object found" );
                aacio_assert( from.balance.amount >= value.amount, "overdrawn balance" );
                
                if( from.balance.amount == value.amount ) {
                    from_acnts.erase( from );
                } else {
                    from_acnts.modify( from, owner, [&]( auto& a ) {
                        a.balance -= value;
                    });
                }
            }

            void add_balance( account_name owner, asset value ) {

                accounts to_acnts( _self, owner );
                auto to = to_acnts.find( value.symbol.name() );

                if( to == to_acnts.end() ) {
                    to_acnts.emplace( _self, [&]( auto& a ){
                        a.balance = value;
                    });
                } else {
                    to_acnts.modify( to, 0, [&]( auto& a ) {
                        a.balance += value;
                    });
                }
            }

            int64_t get_symbol_multiple(symbol_type symbol) {
                int64_t p = (int64_t)symbol.precision();
                int64_t p10 = 1;
                while( p > 0  ) {
                    p10 *= 10; --p;
                }
                return p10;
            }
    };

    AACIO_ABI( quicktoken, (create)(issue)(transfer)(setissuecfg))
}


