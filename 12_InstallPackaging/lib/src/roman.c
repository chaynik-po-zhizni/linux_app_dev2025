#include "roman.h"
#include <string.h>
#include <ctype.h>

/**
 * @file roman.c
 * @brief The implementation of convertion to and from roman numerical system
 * @author Vasilieva Vera
 * @date 2025
 * @version 1.0
 */


int is_valid_roman_number(int num) {
    return (num >= 1 && num <= MAX_ROMAN);
}

int is_maybe_valid_roman_number(const char *roman) {
    int len = strlen(roman);
    int i = 0;
    while (i < len) {
        char c = toupper(roman[i]);
        if (c != 'I' && c != 'V' && c != 'X' && c != 'L' && 
            c != 'C' && c != 'D' && c != 'M') {
                return 0;
        }
        i++;
    }
    return 1; 
}

static int roman_arabic[7] = {1000, 500, 100, 50, 10, 5, 1};
static char arabic_roman[7] = {'M', 'D', 'C', 'L', 'X', 'V', 'I'};

int from_roman(const char *roman) {
    if (!is_maybe_valid_roman_number(roman)) {
        return -1;
    }
    int len = strlen(roman);
    if (len == 0) {
        return -1;
    }
    int idx = 0;
    int idx_s = 0;
    int res = 0;
    char cur, next;
    while (1) {
        if (idx >= 7) {
            return -1;
        }
        if (idx_s >= len) {
            break;
        }
        cur = roman[idx_s];
        next = roman[idx_s + 1];
        if (idx % 2 == 0) {
            if (cur == arabic_roman[idx]) {
                res += roman_arabic[idx];
                idx_s++;
                continue;
            }
            if (idx <= 4 && cur == arabic_roman[idx + 2]) {
                if (next == arabic_roman[idx + 1]) {
                    res += 4 * roman_arabic[idx + 2];
                    idx_s += 2;
                    idx++;
                    continue;
                } else if (next == arabic_roman[idx]) {
                    res += 9 * roman_arabic[idx + 2];
                    idx_s += 2;
                    idx++;
                    continue;
                }
            }
            idx++;
        } else {
            if (cur == arabic_roman[idx]) {
                res += roman_arabic[idx];
                idx_s++;
            }
            idx++;
            continue;
        }
    }
    if (!is_valid_roman_number(res)) {
        return -1;
    }
    return res;
}

int to_roman_without_write(int num) {
    if (!is_valid_roman_number(num)) {
        return 0;
    }
    int idx = 0, r_a_idx = 0;
    int cur = 1000;
    while (1) {
        if (cur <= 0) {
            break;
        }
        int cur_rem = 0;
        cur_rem = num / cur;
        num %= cur;
        if (cur_rem == 4) {
            idx++;
            idx++;
        } else if (cur_rem == 9) {
            idx++;
            idx++;
        } else {
            if (cur_rem >= 5) {
                idx++;
                cur_rem -= 5;
            }
            for (int i = 0; i < cur_rem; i++) {
                idx++;
            }
        } 
        cur /= 10;
        r_a_idx += 2;
    }
    idx++;
    return idx;
}

int to_roman(char *roman_s, int len, int num) {
    if (len < to_roman_without_write(num)) {
        return 0;
    }
    if (!is_valid_roman_number(num)) {
        return 0;
    }
    int idx = 0, r_a_idx = 0;
    int cur = 1000;
    while (1) {
        if (cur <= 0) {
            break;
        }
        int cur_rem = 0;
        cur_rem = num / cur;
        num %= cur;
        if (cur_rem == 4) {
            roman_s[idx++] = arabic_roman[r_a_idx]; 
            roman_s[idx++] = arabic_roman[r_a_idx - 1]; 
        } else if (cur_rem == 9) {
            roman_s[idx++] = arabic_roman[r_a_idx]; 
            roman_s[idx++] = arabic_roman[r_a_idx - 2]; 
        } else {
            if (cur_rem >= 5) {
                roman_s[idx++] = arabic_roman[r_a_idx - 1];
                cur_rem -= 5;
            }
            for (int i = 0; i < cur_rem; i++) {
                roman_s[idx++] = arabic_roman[r_a_idx];
            }
        } 
        cur /= 10;
        r_a_idx += 2;
    }
    roman_s[idx++] = 0;
    return idx;
}
