#include <check.h>
#include "roman.h"

#suite Edge
#tcase Boundary_Cases
#test min_max_values
    ck_assert_int_eq(from_roman("I"), 1);
    ck_assert_int_eq(from_roman("MMMCMXCIX"), 3999);
    
    char buffer[20];
    ck_assert_int_ne(to_roman(buffer, sizeof(buffer), 1), 0);
    ck_assert_int_ne(to_roman(buffer, sizeof(buffer), 3999), 0);

#tcase Invalid_Inputs
#test invalid_characters
    ck_assert_int_eq(from_roman("A"), -1);
    ck_assert_int_eq(from_roman("123"), -1);
    ck_assert_int_eq(from_roman("IXA"), -1);
    ck_assert_int_eq(from_roman("X I"), -1);

#test invalid_valid_sequences
    ck_assert_int_eq(from_roman("IIII"),4);
    ck_assert_int_eq(from_roman("XXXX"), 40);
    ck_assert_int_eq(from_roman("CCCC"), 400);
    ck_assert_int_eq(from_roman("MMMM"), -1);
    ck_assert_int_eq(from_roman("VV"), -1);
    ck_assert_int_eq(from_roman("LL"), -1);
    ck_assert_int_eq(from_roman("DD"), -1);