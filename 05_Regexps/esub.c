#include <regex.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void set_string_color(int x) {
    switch (x) {
        case -1:
        printf("%s", "\033[0m");
        break;
        case 0:
        printf("%s", "\033[0;4m");
        break;
        case 1:
        printf("%s", "\033[0;31m");
        break;
        case 2:
        printf("%s", "\033[0;32m");
        break;
        case 3:
        printf("%s", "\033[0;33m");
        break;
        case 4:
        printf("%s", "\033[0;34m");
        break;
        case 5:
        printf("%s", "\033[0;35m");
        break;
        case 6:
        printf("%s", "\033[0;36m");
        break;
        case 7:
        printf("%s", "\033[0;31;4m");
        break;
        case 8:
        printf("%s", "\033[0;32;4m");
        break;
        case 9:
        printf("%s", "\033[0;34;4m");
        break;
    }
}

struct printable_match_helper {
    int group_num;
    int start;
    int end;
}; 

struct printable_match_helper * add_printable(struct printable_match_helper *arr,
                    size_t *len, int group_num, int start, int end) {
    int old_len = *len;
    *len += 1;
    struct printable_match_helper *res = realloc(arr, (*len) * sizeof(struct printable_match_helper));
    if (res == NULL) {
        return NULL;
    }
    res[*len - 1].end = end;
    res[*len - 1].start = start;
    res[*len - 1].group_num = group_num;
    return res;
}

void error_exit(int flag, regex_t *regex, regmatch_t *matches, struct printable_match_helper *printable, const char* message, int *d) {
    if (!flag) {
        return;
    }
    if (d == NULL) {
        fprintf(stderr, "%s\n", message);
    } else {
        fprintf(stderr, "%s %d\n", message, *d);
    }
    regfree(regex);
    if (matches != NULL)
        free(matches);
    if (printable != NULL)
        free(printable);
    exit(1);
}

int main(int argc, char*argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Error: use <regexp> <substitution> <string>\n");
        exit(1);
    }
    char *regexp = argv[1];
    char *substitution = argv[2];
    char *string = argv[3];
    int err;
    regex_t regex;

    if ((err = regcomp(&regex, regexp, REG_EXTENDED))) {
        size_t err_length = regerror(err, &regex, NULL, 0);
        char *err_str = malloc(err_length * sizeof(char));
        if (err_str == NULL) {
            exit(1);
        }
        regerror(err, &regex, err_str, err_length);
        fprintf(stderr, "Error on regcomp(): %s\n", err_str);
        free(err_str);
        exit(1);
    }

    regmatch_t *matches = malloc(sizeof(regmatch_t) * (regex.re_nsub + 1));
    error_exit(matches == NULL, &regex, NULL, NULL, "Error: malloc not enough memory", NULL);
    int ret = regexec(&regex, string, regex.re_nsub + 1, matches, 0);
    int reg_nomatch = REG_NOMATCH;
    if (ret == reg_nomatch) {
        printf("%s\n", string);
        free(matches);
        regfree(&regex);
        exit(0);
    }
    if (ret != 0) {
        error_exit(matches == NULL, &regex, matches, NULL, "Error: regexec return not 0 and not", &reg_nomatch);
    }

    struct printable_match_helper *result = NULL;
    size_t result_len = 0;
    size_t substitution_len = strlen(substitution);
    size_t i = 0;
    int cur_start = 0;
    while (i < substitution_len) {
        char c = substitution[i];
        size_t new_len = 0;
        if (c == '\\') {
            if (cur_start != -1) {
                result = add_printable(result, &result_len, -1, cur_start, i);
                error_exit(result == NULL, &regex, matches, NULL, "Error: realloc not enough memory", NULL);
            }
            i++;
            char c = substitution[i];
            if (isdigit(c)) {
                int group_num = c - '0';
                error_exit(group_num > regex.re_nsub, &regex, matches, result, "Error: invalid reference ", &group_num);
                result = add_printable(result, &result_len, group_num, matches[group_num].rm_so, matches[group_num].rm_eo);
                error_exit(result == NULL, &regex, matches, NULL, "Error: realloc not enough memory", NULL);
                cur_start = -1;
            } else {
                cur_start = i;
            }
        } else if (c == '&') {
            if (cur_start != -1) {
                result = add_printable(result, &result_len, -1, cur_start, i);
                error_exit(result == NULL, &regex, matches, NULL, "Error: realloc not enough memory", NULL);
            }
            result = add_printable(result, &result_len, 0, matches[0].rm_so, matches[0].rm_eo);
            error_exit(result == NULL, &regex, matches, NULL, "Error: realloc not enough memory", NULL);
            cur_start = -1;
        } else {
            if (cur_start == -1) {
                cur_start = i;
            }
        }
        i++;
    }

    if (cur_start != -1) {
        result = add_printable(result, &result_len, -1, cur_start, i);
        error_exit(result == NULL, &regex, matches, NULL, "Error: realloc not enough memory", NULL);
    }

    int start = matches[0].rm_so;
    int end = matches[0].rm_eo;
    printf("%.*s", start, string);
    for (int j = 0; j < result_len; j++) {
        int curr_len = result[j].end - result[j].start + 1;
        char *current = malloc(curr_len);
        error_exit(current == NULL, &regex, matches, result, "Error: malloc not enough memory", NULL);
        current[curr_len - 1] = 0;
        char *s = result[j].group_num == -1?substitution:string;
        strncpy(current, s + result[j].start, curr_len - 1);
        set_string_color(result[j].group_num);
        printf("%s", current);
        free(current);
    }
    set_string_color(-1);
    printf("%s\n", string + end);

    free(matches);
    regfree(&regex);
    free(result);
}