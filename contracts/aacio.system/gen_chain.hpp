/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */
#pragma once

#include <aaciolib/asset.hpp>
#include <aaciolib/aacio.hpp>

#include <string>

namespace aaciosystem {

   using std::string;
   using aacio::asset;

   struct bidder_info {
      account_name  bidder;
      asset         price;
      bool          is_generated = false;

      bidder_info() {}
      bidder_info(account_name b, asset p) : bidder(b), price(p) {}
      bidder_info(const bidder_info& bi) {
          bidder       = bi.bidder;
          price        = bi.price;
          is_generated = bi.is_generated;
      }
      ~bidder_info() {}

      bidder_info& operator=(const bidder_info& b) {
          bidder = b.bidder;
          price = b.price;
          is_generated = b.is_generated;
          return *this;
      }

      friend bool operator==( const bidder_info& a, const bidder_info& b ) {
          return a.price == b.price;
      }

      friend bool operator!=( const bidder_info& a, const bidder_info& b ) {
          return !( a == b);
      }

      friend bool operator<( const bidder_info& a, const bidder_info& b ) {
          return a.price < b.price;
      }
      uint64_t primary_key()const { return bidder; }

      AACLIB_SERIALIZE( bidder_info, (bidder)(price)(is_generated) )
   };

   struct bid_info {
      uint64_t     id;      //format: YYYYWW, Y - year, W - Week
      bool         is_ongoing = true;
      vector<bidder_info>  bidders;

      uint64_t primary_key()const { return id; }
   };

   struct chain_info {
      account_name issuer;
      string       serial_number;
      string       token_symbl;

      uint64_t primary_key()const { return issuer; }
   };

   typedef aacio::multi_index<N(chainbids), bid_info> chainbids;
   typedef aacio::multi_index<N(chains), chain_info> chains;

} /// namespace aaciosystem
