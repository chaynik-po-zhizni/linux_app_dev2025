#include <check.h>
#include "roman.h"


#suite Basic

#tcase Simple_Conversions
#test simple_numbers
    ck_assert_int_eq(from_roman("I"), 1);
    ck_assert_int_eq(from_roman("II"), 2);
    ck_assert_int_eq(from_roman("III"), 3);
    ck_assert_int_eq(from_roman("VI"), 6);
    ck_assert_int_eq(from_roman("VII"), 7);
    ck_assert_int_eq(from_roman("VIII"), 8);

#tcase Subtractive_Notation
#test subtractive_cases
    ck_assert_int_eq(from_roman("IV"), 4);
    ck_assert_int_eq(from_roman("IX"), 9);
    ck_assert_int_eq(from_roman("XL"), 40);
    ck_assert_int_eq(from_roman("XC"), 90);
    ck_assert_int_eq(from_roman("CD"), 400);
    ck_assert_int_eq(from_roman("CM"), 900);