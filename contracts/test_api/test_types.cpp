/**
 *  @file
 *  @copyright defined in Acute-Angle-Chain/LICENSE.txt
 */
#include <aaciolib/aacio.hpp>

#include "test_api.hpp"

void test_types::types_size() {

   aacio_assert( sizeof(int64_t) == 8, "int64_t size != 8");
   aacio_assert( sizeof(uint64_t) ==  8, "uint64_t size != 8");
   aacio_assert( sizeof(uint32_t) ==  4, "uint32_t size != 4");
   aacio_assert( sizeof(int32_t) ==  4, "int32_t size != 4");
   aacio_assert( sizeof(uint128_t) == 16, "uint128_t size != 16");
   aacio_assert( sizeof(int128_t) == 16, "int128_t size != 16");
   aacio_assert( sizeof(uint8_t) ==  1, "uint8_t size != 1");

   aacio_assert( sizeof(account_name) ==  8, "account_name size !=  8");
   aacio_assert( sizeof(token_name) ==  8, "token_name size !=  8");
   aacio_assert( sizeof(table_name) ==  8, "table_name size !=  8");
   aacio_assert( sizeof(time) ==  4, "time size !=  4");
   aacio_assert( sizeof(aacio::key256) == 32, "key256 size != 32" );
}

void test_types::char_to_symbol() {

   aacio_assert( aacio::char_to_symbol('1') ==  1, "aacio::char_to_symbol('1') !=  1");
   aacio_assert( aacio::char_to_symbol('2') ==  2, "aacio::char_to_symbol('2') !=  2");
   aacio_assert( aacio::char_to_symbol('3') ==  3, "aacio::char_to_symbol('3') !=  3");
   aacio_assert( aacio::char_to_symbol('4') ==  4, "aacio::char_to_symbol('4') !=  4");
   aacio_assert( aacio::char_to_symbol('5') ==  5, "aacio::char_to_symbol('5') !=  5");
   aacio_assert( aacio::char_to_symbol('a') ==  6, "aacio::char_to_symbol('a') !=  6");
   aacio_assert( aacio::char_to_symbol('b') ==  7, "aacio::char_to_symbol('b') !=  7");
   aacio_assert( aacio::char_to_symbol('c') ==  8, "aacio::char_to_symbol('c') !=  8");
   aacio_assert( aacio::char_to_symbol('d') ==  9, "aacio::char_to_symbol('d') !=  9");
   aacio_assert( aacio::char_to_symbol('e') == 10, "aacio::char_to_symbol('e') != 10");
   aacio_assert( aacio::char_to_symbol('f') == 11, "aacio::char_to_symbol('f') != 11");
   aacio_assert( aacio::char_to_symbol('g') == 12, "aacio::char_to_symbol('g') != 12");
   aacio_assert( aacio::char_to_symbol('h') == 13, "aacio::char_to_symbol('h') != 13");
   aacio_assert( aacio::char_to_symbol('i') == 14, "aacio::char_to_symbol('i') != 14");
   aacio_assert( aacio::char_to_symbol('j') == 15, "aacio::char_to_symbol('j') != 15");
   aacio_assert( aacio::char_to_symbol('k') == 16, "aacio::char_to_symbol('k') != 16");
   aacio_assert( aacio::char_to_symbol('l') == 17, "aacio::char_to_symbol('l') != 17");
   aacio_assert( aacio::char_to_symbol('m') == 18, "aacio::char_to_symbol('m') != 18");
   aacio_assert( aacio::char_to_symbol('n') == 19, "aacio::char_to_symbol('n') != 19");
   aacio_assert( aacio::char_to_symbol('o') == 20, "aacio::char_to_symbol('o') != 20");
   aacio_assert( aacio::char_to_symbol('p') == 21, "aacio::char_to_symbol('p') != 21");
   aacio_assert( aacio::char_to_symbol('q') == 22, "aacio::char_to_symbol('q') != 22");
   aacio_assert( aacio::char_to_symbol('r') == 23, "aacio::char_to_symbol('r') != 23");
   aacio_assert( aacio::char_to_symbol('s') == 24, "aacio::char_to_symbol('s') != 24");
   aacio_assert( aacio::char_to_symbol('t') == 25, "aacio::char_to_symbol('t') != 25");
   aacio_assert( aacio::char_to_symbol('u') == 26, "aacio::char_to_symbol('u') != 26");
   aacio_assert( aacio::char_to_symbol('v') == 27, "aacio::char_to_symbol('v') != 27");
   aacio_assert( aacio::char_to_symbol('w') == 28, "aacio::char_to_symbol('w') != 28");
   aacio_assert( aacio::char_to_symbol('x') == 29, "aacio::char_to_symbol('x') != 29");
   aacio_assert( aacio::char_to_symbol('y') == 30, "aacio::char_to_symbol('y') != 30");
   aacio_assert( aacio::char_to_symbol('z') == 31, "aacio::char_to_symbol('z') != 31");

   for(unsigned char i = 0; i<255; i++) {
      if((i >= 'a' && i <= 'z') || (i >= '1' || i <= '5')) continue;
      aacio_assert( aacio::char_to_symbol((char)i) == 0, "aacio::char_to_symbol() != 0");
   }
}

void test_types::string_to_name() {

   aacio_assert( aacio::string_to_name("a") == N(a) , "aacio::string_to_name(a)" );
   aacio_assert( aacio::string_to_name("ba") == N(ba) , "aacio::string_to_name(ba)" );
   aacio_assert( aacio::string_to_name("cba") == N(cba) , "aacio::string_to_name(cba)" );
   aacio_assert( aacio::string_to_name("dcba") == N(dcba) , "aacio::string_to_name(dcba)" );
   aacio_assert( aacio::string_to_name("edcba") == N(edcba) , "aacio::string_to_name(edcba)" );
   aacio_assert( aacio::string_to_name("fedcba") == N(fedcba) , "aacio::string_to_name(fedcba)" );
   aacio_assert( aacio::string_to_name("gfedcba") == N(gfedcba) , "aacio::string_to_name(gfedcba)" );
   aacio_assert( aacio::string_to_name("hgfedcba") == N(hgfedcba) , "aacio::string_to_name(hgfedcba)" );
   aacio_assert( aacio::string_to_name("ihgfedcba") == N(ihgfedcba) , "aacio::string_to_name(ihgfedcba)" );
   aacio_assert( aacio::string_to_name("jihgfedcba") == N(jihgfedcba) , "aacio::string_to_name(jihgfedcba)" );
   aacio_assert( aacio::string_to_name("kjihgfedcba") == N(kjihgfedcba) , "aacio::string_to_name(kjihgfedcba)" );
   aacio_assert( aacio::string_to_name("lkjihgfedcba") == N(lkjihgfedcba) , "aacio::string_to_name(lkjihgfedcba)" );
   aacio_assert( aacio::string_to_name("mlkjihgfedcba") == N(mlkjihgfedcba) , "aacio::string_to_name(mlkjihgfedcba)" );
   aacio_assert( aacio::string_to_name("mlkjihgfedcba1") == N(mlkjihgfedcba2) , "aacio::string_to_name(mlkjihgfedcba2)" );
   aacio_assert( aacio::string_to_name("mlkjihgfedcba55") == N(mlkjihgfedcba14) , "aacio::string_to_name(mlkjihgfedcba14)" );

   aacio_assert( aacio::string_to_name("azAA34") == N(azBB34) , "aacio::string_to_name N(azBB34)" );
   aacio_assert( aacio::string_to_name("AZaz12Bc34") == N(AZaz12Bc34) , "aacio::string_to_name AZaz12Bc34" );
   aacio_assert( aacio::string_to_name("AAAAAAAAAAAAAAA") == aacio::string_to_name("BBBBBBBBBBBBBDDDDDFFFGG") , "aacio::string_to_name BBBBBBBBBBBBBDDDDDFFFGG" );
}

void test_types::name_class() {

   aacio_assert( aacio::name{aacio::string_to_name("azAA34")}.value == N(azAA34), "aacio::name != N(azAA34)" );
   aacio_assert( aacio::name{aacio::string_to_name("AABBCC")}.value == 0, "aacio::name != N(0)" );
   aacio_assert( aacio::name{aacio::string_to_name("AA11")}.value == N(AA11), "aacio::name != N(AA11)" );
   aacio_assert( aacio::name{aacio::string_to_name("11AA")}.value == N(11), "aacio::name != N(11)" );
   aacio_assert( aacio::name{aacio::string_to_name("22BBCCXXAA")}.value == N(22), "aacio::name != N(22)" );
   aacio_assert( aacio::name{aacio::string_to_name("AAAbbcccdd")} == aacio::name{aacio::string_to_name("AAAbbcccdd")}, "aacio::name == aacio::name" );

   uint64_t tmp = aacio::name{aacio::string_to_name("11bbcccdd")};
   aacio_assert(N(11bbcccdd) == tmp, "N(11bbcccdd) == tmp");
}
