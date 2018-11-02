/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */

#include "aacio.token.hpp"

namespace aacio {

void token::create( account_name issuer,
                    asset        maximum_supply )
{
    require_auth( _self );

    auto sym = maximum_supply.symbol;
    aacio_assert( sym.is_valid(), "invalid symbol name" );
    aacio_assert( maximum_supply.is_valid(), "invalid supply");
    aacio_assert( maximum_supply.amount > 0, "max-supply must be positive");

    stats statstable( _self, sym.name() );
    auto existing = statstable.find( sym.name() );
    aacio_assert( existing == statstable.end(), "token with symbol already exists" );

    statstable.emplace( _self, [&]( auto& s ) {
       s.supply.symbol = maximum_supply.symbol;
       s.max_supply    = maximum_supply;
       s.issuer        = issuer;
    });
}


void token::issue( account_name to, asset quantity, string memo )
{
    auto sym = quantity.symbol;
    aacio_assert( sym.is_valid(), "invalid symbol name" );
    aacio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    auto sym_name = sym.name();
    stats statstable( _self, sym_name );
    auto existing = statstable.find( sym_name );
    aacio_assert( existing != statstable.end(), "token with symbol does not exist, create token before issue" );
    const auto& st = *existing;

    require_auth( st.issuer );
    aacio_assert( quantity.is_valid(), "invalid quantity" );
    aacio_assert( quantity.amount > 0, "must issue positive quantity" );

    aacio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    aacio_assert( quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

    statstable.modify( st, 0, [&]( auto& s ) {
       s.supply += quantity;
    });

    add_balance( st.issuer, quantity, st.issuer );

    if( to != st.issuer ) {
       SEND_INLINE_ACTION( *this, transfer, {st.issuer,N(active)}, {st.issuer, to, quantity, memo} );
    }
}

void token::transfer( account_name from,
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
    add_balance( to, quantity, from );
}

void token::consume( account_name from, asset quantity, string memo )
{
    require_auth( from );
    auto sym = quantity.symbol.name();
    stats statstable( _self, sym );
    const auto& st = statstable.get( sym );

    require_recipient( from );
    aacio_assert( quantity.is_valid(), "invalid quantity" );
    aacio_assert( quantity.amount > 0, "must transfer positive quantity" );
    aacio_assert( quantity.symbol == st.supply.symbol, "symbol precision mismatch" );
    aacio_assert( memo.size() <= 256, "memo has more than 256 bytes" );

    sub_balance( from, quantity );

    statstable.modify( st, 0, [&]( auto& s ) {
       s.supply -= quantity;
    });
}

void token::exchange( account_name payer, asset quantity )
{
   require_auth( payer );

   config configtable( _self, _self );
   token_config cfg;
   if ( configtable.exists() )
      cfg = configtable.get();

   asset fee = quantity;
   fee.amount = quantity.amount * cfg.fee / 10000;
   asset quant_after_fee = quantity;
   quant_after_fee.amount -= fee.amount;
   asset sn(0, RAM_TRADE_SYMBOL);
   sn.amount = static_cast<int64_t>(quant_after_fee.amount / cfg.rate);

   aacio_assert( (quant_after_fee.amount > 0) && (sn.amount > 0) , "insufficient token to exchange" );

   if (fee.amount > 0) {
      SEND_INLINE_ACTION( *this, transfer, {payer,N(active)}, {payer, N(aacio.snfee), fee, "exchange SN fee"} );
   }

   SEND_INLINE_ACTION( *this, transfer, {payer,N(active)}, {payer, N(aacio.sn), quant_after_fee, "exchange SN"} );
   SEND_INLINE_ACTION( *this, transfer, {N(aacio.sn),N(active)}, {N(aacio.sn), payer, sn, "exchange SN"} );
}

void token::setrate( double rate )
{
   require_auth( _self );

   config configtable( _self, _self );
   token_config cfg;
   if ( configtable.exists() )
      cfg = configtable.get();

   cfg.rate = rate;
   configtable.set( cfg, _self );
}

void token::setfee( uint32_t fee )
{
   require_auth( _self );

   aacio_assert( fee <= 10000, "must between 0...10000" );
   config configtable(_self, _self);
   token_config cfg;
   if ( configtable.exists() )
      cfg = configtable.get();

   cfg.fee = fee;
   configtable.set( cfg, _self );
}

void token::sub_balance( account_name owner, asset value ) {
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

void token::add_balance( account_name owner, asset value, account_name ram_payer )
{
   accounts to_acnts( _self, owner );
   auto to = to_acnts.find( value.symbol.name() );
   if( to == to_acnts.end() ) {
      to_acnts.emplace( ram_payer, [&]( auto& a ){
        a.balance = value;
      });
   } else {
      to_acnts.modify( to, 0, [&]( auto& a ) {
        a.balance += value;
      });
   }
}

} /// namespace aacio

AACIO_ABI( aacio::token, (create)(issue)(transfer)(consume)(exchange)(setrate)(setfee) )
