#pragma once
#include <aaciolib/serialize.hpp>
#include <aaciolib/print.hpp>
#include <aaciolib/system.h>
#include <aaciolib/symbol.hpp>
#include <tuple>
#include <limits>

namespace aacio {

   struct asset {
      int64_t      amount;
      symbol_type  symbol;

      static constexpr int64_t max_amount    = (1LL << 62) - 1;

      explicit asset( int64_t a = 0, symbol_type s = CORE_SYMBOL )
      :amount(a),symbol{s}
      {
         aacio_assert( is_amount_within_range(), "magnitude of asset amount must be less than 2^62" );
         aacio_assert( symbol.is_valid(),        "invalid symbol name" );
      }

      bool is_amount_within_range()const { return -max_amount <= amount && amount <= max_amount; }
      bool is_valid()const               { return is_amount_within_range() && symbol.is_valid(); }

      void set_amount( int64_t a ) {
         amount = a;
         aacio_assert( is_amount_within_range(), "magnitude of asset amount must be less than 2^62" );
      }

      asset operator-()const {
         asset r = *this;
         r.amount = -r.amount;
         return r;
      }

      asset& operator-=( const asset& a ) {
         aacio_assert( a.symbol == symbol, "attempt to subtract asset with different symbol" );
         amount -= a.amount;
         aacio_assert( -max_amount <= amount, "subtraction underflow" );
         aacio_assert( amount <= max_amount,  "subtraction overflow" );
         return *this;
      }

      asset& operator+=( const asset& a ) {
         aacio_assert( a.symbol == symbol, "attempt to add asset with different symbol" );
         amount += a.amount;
         aacio_assert( -max_amount <= amount, "addition underflow" );
         aacio_assert( amount <= max_amount,  "addition overflow" );
         return *this;
      }

      inline friend asset operator+( const asset& a, const asset& b ) {
         asset result = a;
         result += b;
         return result;
      }

      inline friend asset operator-( const asset& a, const asset& b ) {
         asset result = a;
         result -= b;
         return result;
      }

      asset& operator*=( int64_t a ) {
         aacio_assert( a == 0 || (amount * a) / a == amount, "multiplication overflow or underflow" );
         aacio_assert( -max_amount <= amount, "multiplication underflow" );
         aacio_assert( amount <= max_amount,  "multiplication overflow" );
         amount *= a;
         return *this;
      }

      friend asset operator*( const asset& a, int64_t b ) {
         asset result = a;
         result *= b;
         return result;
      }

      friend asset operator*( int64_t b, const asset& a ) {
         asset result = a;
         result *= b;
         return result;
      }

      asset& operator/=( int64_t a ) {
         amount /= a;
         return *this;
      }

      friend asset operator/( const asset& a, int64_t b ) {
         asset result = a;
         result /= b;
         return result;
      }

      friend int64_t operator/( const asset& a, const asset& b ) {
         aacio_assert( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount / b.amount;
      }

      friend bool operator==( const asset& a, const asset& b ) {
         aacio_assert( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount == b.amount;
      }

      friend bool operator!=( const asset& a, const asset& b ) {
         return !( a == b);
      }

      friend bool operator<( const asset& a, const asset& b ) {
         aacio_assert( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount < b.amount;
      }

      friend bool operator<=( const asset& a, const asset& b ) {
         aacio_assert( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount <= b.amount;
      }

      friend bool operator>( const asset& a, const asset& b ) {
         aacio_assert( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount > b.amount;
      }

      friend bool operator>=( const asset& a, const asset& b ) {
         aacio_assert( a.symbol == b.symbol, "comparison of assets with different symbols is not allowed" );
         return a.amount >= b.amount;
      }

      void print()const {
         int64_t p = (int64_t)symbol.precision();
         int64_t p10 = 1;
         while( p > 0  ) {
            p10 *= 10; --p;
         }
         p = (int64_t)symbol.precision();

         char fraction[p+1];
         fraction[p] = '\0';
         auto change = amount % p10;

         for( int64_t i = p -1; i >= 0; --i ) {
            fraction[i] = (change % 10) + '0';
            change /= 10;
         }
         printi( amount / p10 );
         prints(".");
         prints_l( fraction, uint32_t(p) );
         prints(" ");
         symbol.print(false);
      }

      AACLIB_SERIALIZE( asset, (amount)(symbol) )
   };

   struct extended_asset : public asset {
      account_name contract;

      extended_symbol get_extended_symbol()const { return extended_symbol( symbol, contract ); }
      extended_asset() = default;
      extended_asset( int64_t v, extended_symbol s ):asset(v,s),contract(s.contract){}
      extended_asset( asset a, account_name c ):asset(a),contract(c){}

      void print()const {
         asset::print();
         prints("@");
         printn(contract);
      }
      extended_asset operator-()const {
         asset r = this->asset::operator-();
         return {r, contract};
      }

      friend extended_asset operator - ( const extended_asset& a, const extended_asset& b ) {
         aacio_assert( a.contract == b.contract, "type mismatch" );
         asset r = static_cast<const asset&>(a) - static_cast<const asset&>(b);
         return {r, a.contract};
      }

      friend extended_asset operator + ( const extended_asset& a, const extended_asset& b ) {
         aacio_assert( a.contract == b.contract, "type mismatch" );
         asset r = static_cast<const asset&>(a) + static_cast<const asset&>(b);
         return {r, a.contract};
      }

      AACLIB_SERIALIZE( extended_asset, (amount)(symbol)(contract) )
   };


} /// namespace aacio
