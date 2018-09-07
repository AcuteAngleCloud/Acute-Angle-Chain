/**
 *  @file
 *  @copyright defined in aac/LICENSE.txt
 */
#pragma once

#include <aaciolib/aacio.hpp>
#include <aaciolib/token.hpp>
#include <aaciolib/reflect.hpp>
#include <aaciolib/generic_currency.hpp>

#include <bancor/converter.hpp>
#include <currency/currency.hpp>

namespace bancor {
   typedef aacio::generic_currency< aacio::token<N(other),S(4,OTHER)> >  other_currency;
   typedef aacio::generic_currency< aacio::token<N(bancor),S(4,RELAY)> > relay_currency;
   typedef aacio::generic_currency< aacio::token<N(currency),S(4,CUR)> > cur_currency;

   typedef converter<relay_currency, other_currency, cur_currency > example_converter;
} /// bancor

