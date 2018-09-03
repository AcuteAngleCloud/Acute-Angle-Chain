/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */

#include "gen_chain.hpp"
#include "ctime"

namespace aaciosystem {

const int BID_FAIL    = 0;
const int BID_SUCCESS = 1;
const int BID_ONGOING = 2;
const int BID_USED    = 3;
const int64_t base_price = 1000000000;

int system_contract::check_bid_result( uint64_t id, account_name bidder ) {
    chainbids bidstable( _self, id );

    auto b_info = bidstable.find( id );
    if ( b_info == bidstable.end() ) return BID_FAIL;
    if ( b_info->is_ongoing ) return BID_ONGOING;

    auto ib = find_if(b_info->bidders.begin(), b_info->bidders.end(),
                      [&] (const bidder_info& b) { return b.bidder == bidder; });
    if ( ib == b_info->bidders.end() ) return BID_FAIL;
    if ( ib->is_generated ) return BID_USED;

    return BID_SUCCESS;
}

void system_contract::bidchain( uint64_t id, account_name bidder, asset price )
{
    require_auth( bidder );

    aacio_assert( (id > 200000) && (id < 999999), "invalid bid id" );
    aacio_assert( price.symbol == asset().symbol, "asset must be system token" );
    aacio_assert( price.amount >= base_price, "must greater than base price" );
    chainbids bidstable( _self, id );

    auto b_info = bidstable.find( id );
    aacio_assert( b_info != bidstable.end(), "the bid id does not exist" );
    aacio_assert( b_info->is_ongoing, "the bidding has closed" );
    /*if ( b_info == bidstable.end() ) {
        INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {bidder,N(active)},
                             { bidder, N(aacio.bchain), price, std::string("bid chain ") + std::to_string(id) } );
        bidstable.emplace( _self, [&]( auto& b ) {
           b.id         = id;
           b.is_ongoing = true;
           b.bidders.emplace_back(bidder, price);
        });
        return;
    }*/

    auto ibidder = find_if(b_info->bidders.begin(), b_info->bidders.end(),
                      [&] (const bidder_info& b) { return b.bidder == bidder; });
    if ( ibidder != b_info->bidders.end() ) {  //the account has already bidden
        auto min = min_element(b_info->bidders.begin(), b_info->bidders.end());
        aacio_assert( (price.amount - ibidder->price.amount) >= (min->price.amount / 100), "must increase bid by 1%" );
        bidstable.modify( b_info, 0, [&]( auto& b ) {
           auto ib = find_if(b.bidders.begin(), b.bidders.end(),
                   [&] (const bidder_info& bi) { return bi.bidder == bidder; });
           if ( ib == b.bidders.end() ) return;

           INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {bidder,N(active)},
                             { bidder, N(aacio.bchain), price - ib->price, std::string("increase bid ") + std::to_string(id) } );
           ib->price = price;
           stable_sort(b.bidders.rbegin(), b.bidders.rend());
        });
        return;
    }

    if (b_info->bidders.size() < _gstate.current_chain_quota) {
        INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {bidder,N(active)},
                             { bidder, N(aacio.bchain), price, std::string("bid chain ") + std::to_string(id) } );
        bidstable.modify( b_info, 0, [&]( auto& b ) {
           b.bidders.emplace_back(bidder, price);
           stable_sort(b.bidders.rbegin(), b.bidders.rend());
        });
    }
    else {
        bidstable.modify( b_info, 0, [&]( auto& b ) {
           auto min = min_element(b.bidders.begin(), b.bidders.end());
           aacio_assert( (price.amount - min->price.amount) >= (min->price.amount / 100), "must increase bid by 1%" );
           INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {bidder,N(active)},
                             { bidder, N(aacio.bchain), price, std::string("bid chain ") + std::to_string(id) } );
           INLINE_ACTION_SENDER(aacio::token, transfer)( N(aacio.token), {N(aacio.bchain),N(active)},
                             { N(aacio.bchain), min->bidder, min->price, std::string("refund bid on chain auction ") + std::to_string(id) } );
           min->bidder = bidder;
           min->price = price;
           stable_sort(b.bidders.rbegin(), b.bidders.rend());
        });
    }
}

void system_contract::genchain( account_name issuer, uint64_t id, string serial_number, string token_symbl )
{
    require_auth( issuer );

    chains chainstable( _self, issuer );
    auto existing = chainstable.find( issuer );
    aacio_assert( existing == chainstable.end(), "chain has been generated for the account" );
    aacio_assert( (id > 200000) && (id < 999999), "invalid bid id" );
    aacio_assert( serial_number.size() == 64, "invalid serial number" );
    aacio_assert( is_valid_symbol(token_symbl), "invalid token symbol" );
    aacio_assert( check_bid_result(id, issuer) == BID_SUCCESS, "the account fails to bid a chain or the bidding is ongoing" );

    chainstable.emplace( _self, [&]( auto& c ) {
       c.issuer        = issuer;
       c.serial_number = serial_number;
       c.token_symbl   = token_symbl;
    });

    chainbids bidstable( _self, id );
    auto b_info = bidstable.find( id );
    aacio_assert( b_info != bidstable.end(), "the bid id does not exist" );

    bidstable.modify( b_info, 0, [&]( auto& b ) {
       auto ib = find_if(b.bidders.begin(), b.bidders.end(),
                      [&] (const bidder_info& bi) { return bi.bidder == issuer; });
       if ( ib == b.bidders.end() ) return;
       ib->is_generated = true;
    });
}

void system_contract::setquota( uint32_t quota )
{
    require_auth( _self );
    aacio_assert( quota <= 100, "must between 0...100" );

    _gstate.bid_chain_quota = quota;
    _global.set( _gstate, _self );
}

void system_contract::maybe_start_bid_chain() {
    std::time_t t = static_cast<std::time_t>(current_time()/1000000);
    std::tm* gt = std::gmtime(&t);
    /* the period of bidding is 1 week. It is for release only.*/
    if ( !((gt->tm_wday == 0) && (gt->tm_hour == 0)) ) {  //not between sunday 00:00 ~ 00:59
       return;
    }

    uint64_t week_id = static_cast<uint64_t>((gt->tm_year + 1900) * 100 + gt->tm_yday / 7);

    /* the period of bidding is 1 hour. It is for test only.*/
   /* uint64_t week_id = static_cast<uint64_t>(200000 + gt->tm_yday * 100 + gt->tm_hour); */

    /* the period of bidding is 10 minutes. It is for demo only.
    uint64_t week_id = static_cast<uint64_t>(20 * 10000 + gt->tm_hour * 100 + gt->tm_min / 10);*/

    chainbids bidstable( _self, week_id );
    auto b_info = bidstable.find( week_id );
    if ( b_info != bidstable.end() ) {
       return;
    }

    //close the bid for last week
    uint64_t last_week_id;
    if ((week_id % 100) == 0)
        last_week_id = week_id - 48;//48 for release; 77 for test; 95 for demo
    else 
        last_week_id = week_id - 1;

    chainbids bidstable1( _self, last_week_id );
    b_info = bidstable1.find( last_week_id );
    if ( b_info == bidstable1.end() ) {
       //a year may have 52 or 53 sundays
       --last_week_id;
       chainbids bidstable2( _self, last_week_id );
       b_info = bidstable2.find( last_week_id );
       if ( b_info != bidstable2.end() ) {
          bidstable2.modify( b_info, 0, [&]( auto& b ) {
             b.is_ongoing = false;           
          });
       }
    }
    else {
       bidstable1.modify( b_info, 0, [&]( auto& b ) {
          b.is_ongoing = false;           
       });
    }

    _gstate.current_chain_quota = _gstate.bid_chain_quota;
    if (_gstate.current_chain_quota == 0) {  //Bidding is paused in this week
       return;
    }

    //start a bid for this week
    bidstable.emplace( _self, [&]( auto& b ) {
       b.id         = week_id;
       b.is_ongoing = true;
    });
}

} /// namespace aaciosystem

