#include <stdio.h>
#include <stdint.h>

#include "buf.h"

float *a;


#suite Memory
#tcase initialization
#test init_free_test
    a = 0;
    ck_assert_int_eq(buf_capacity(a), 0);
    ck_assert_int_eq(buf_size(a), 0);
    buf_push(a, 1.3f);
    ck_assert_int_eq(buf_size(a), 1);
    ck_assert_float_eq(a[0], 1.3f);
    buf_clear(a);
    ck_assert_int_eq(buf_size(a), 0);
    ck_assert_ptr_ne(a, 0);
    buf_free(a);
    ck_assert_ptr_eq(a, 0);

#tcase сlearing
#test clear_null_ptr_test
    buf_clear(a);   
    ck_assert_int_eq(buf_size(a), 0);
    ck_assert_ptr_eq(a, 0);
