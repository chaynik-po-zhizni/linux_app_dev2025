#include <stdio.h>
#include <stdint.h>

#include "buf.h"


float *a;
long *ai;

void buf_free_ai(void) {
    buf_free(ai);
}
void buf_free_a(void) {
    buf_free(a);
}

#suite Operations
#tcase push_at
#test push_at_operator_test
    for (int i = 0; i < 10000; i++)
        buf_push(ai, i);
    ck_assert_int_eq(buf_size(ai), 10000);
    int match = 0;
    for (int i = 0; i < (int)(buf_size(ai)); i++)
        match += ai[i] == i;
    ck_assert_int_eq(match, 10000);


#tcase grow_trunc
#test grow_trunc_test
    /* buf_grow(), buf_trunc() */
    buf_grow(ai, 1000);
    ck_assert_int_eq(buf_capacity(ai), 1000);
    ck_assert_int_eq(buf_size(ai), 0);
    buf_trunc(ai, 100);
    ck_assert_int_eq(buf_capacity(ai), 100);

#tcase pop
#test pop_test
    buf_push(a, 1.1);
    buf_push(a, 1.2);
    buf_push(a, 1.3);
    buf_push(a, 1.4);
    ck_assert_int_eq(buf_size(a), 4);
    ck_assert_float_eq(buf_pop(a), 1.4f);
    buf_trunc(a, 3);
    ck_assert_int_eq(buf_size(a), 3);
    ck_assert_float_eq(buf_pop(a), 1.3f);
    ck_assert_float_eq(buf_pop(a), 1.2f);
    ck_assert_float_eq(buf_pop(a), 1.1f);
    ck_assert_int_eq(buf_size(a), 0);

#main-pre
        tcase_add_checked_fixture(tc1_3, NULL, buf_free_a);
        tcase_add_checked_fixture(tc1_2, NULL, buf_free_ai);
        tcase_add_checked_fixture(tc1_1, NULL, buf_free_ai);