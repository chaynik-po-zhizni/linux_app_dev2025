#include <stdio.h>
#include <stdint.h>

#include "buf.h"

float *a;


#suite New
#tcase overflow_push
#test overflow_push_test
    a = 0;
    buf_grow(a, 100);
    ck_assert_ptr_ne(a, 0);
    for (int i = 0; i < 100; i++) {
        buf_push(a, (float)i);
    }
    ck_assert_int_eq(buf_size(a), 100);
    struct buf *p = (struct buf *)((char *)a - offsetof(struct buf, buffer));
    float *old_buffer = a;
    size_t old_size = p->size;
    struct buf *new_p = realloc(p, sizeof(struct buf) + sizeof(float) * 50);
    ck_assert_ptr_ne(new_p, 0);
    
    new_p->capacity = 50;
    new_p->size = old_size;
    a = (float *) new_p->buffer;
    float *result = buf_grow1(a, sizeof(float), 0);
    ck_assert_ptr_ne(result, NULL);
    p = (struct buf *)((char *)a - offsetof(struct buf, buffer));
    ck_assert_int_eq(p->capacity, 50);
    ck_assert_int_eq(p->size, 50);
    free(new_p);

