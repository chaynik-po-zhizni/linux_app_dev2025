#include <check.h>
#include <stdio.h>
#include "roman.h"

#suite Roman

#tcase Conversion
#test test_from_roman_basic
    ck_assert_int_eq(from_roman("I"), 1);
    ck_assert_int_eq(from_roman("IV"), 4);
    ck_assert_int_eq(from_roman("V"), 5);
    ck_assert_int_eq(from_roman("IX"), 9);
    ck_assert_int_eq(from_roman("X"), 10);

#test test_from_roman_complex
    ck_assert_int_eq(from_roman("XL"), 40);
    ck_assert_int_eq(from_roman("L"), 50);
    ck_assert_int_eq(from_roman("XC"), 90);
    ck_assert_int_eq(from_roman("C"), 100);
    ck_assert_int_eq(from_roman("CD"), 400);
    ck_assert_int_eq(from_roman("D"), 500);
    ck_assert_int_eq(from_roman("CM"), 900);
    ck_assert_int_eq(from_roman("M"), 1000);
    ck_assert_int_eq(from_roman("MMMCMXCIX"), 3999);

#test test_from_roman_invalid
    ck_assert_int_eq(from_roman("IIII"), 4);
    ck_assert_int_eq(from_roman("VV"), -1);
    ck_assert_int_eq(from_roman(""), -1);
    ck_assert_int_eq(from_roman("ABC"), -1);

#tcase To_Roman
#test test_to_roman_basic
    char buffer[20];
    int result;
    
    result = to_roman(buffer, sizeof(buffer), 1);
    ck_assert_int_eq(result, 2);
    ck_assert_str_eq(buffer, "I");
    
    result = to_roman(buffer, sizeof(buffer), 4);
    ck_assert_int_eq(result, 3);
    ck_assert_str_eq(buffer, "IV");
    
    result = to_roman(buffer, sizeof(buffer), 9);
    ck_assert_int_eq(result, 3);
    ck_assert_str_eq(buffer, "IX");

#test test_to_roman_complex
    char buffer[20];
    int result;
    
    result = to_roman(buffer, sizeof(buffer), 58);
    ck_assert_int_eq(result, 6);
    ck_assert_str_eq(buffer, "LVIII");
    
    result = to_roman(buffer, sizeof(buffer), 1994);
    ck_assert_int_eq(result, 8);
    ck_assert_str_eq(buffer, "MCMXCIV");
    
    result = to_roman(buffer, sizeof(buffer), 3999);
    ck_assert_int_eq(result, 10);
    ck_assert_str_eq(buffer, "MMMCMXCIX");

#test test_to_roman_edge
    char buffer[20];
    int result;
    
    result = to_roman(buffer, 5, 1994);
    ck_assert_int_eq(result, 0);
    
    result = to_roman(buffer, sizeof(buffer), 0);
    ck_assert_int_eq(result, 0);
    
    result = to_roman(buffer, sizeof(buffer), 4000);
    ck_assert_int_eq(result, 0);
