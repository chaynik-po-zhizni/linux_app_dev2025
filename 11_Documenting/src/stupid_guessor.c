#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libgen.h>
#include <libintl.h>
#include <argp.h>


/**
 * @file stupid_guessor.c
 * @brief A number guessing program with Roman numerals support
 * @author Vasilieva Vera
 * @date 2025
 * @version 2.0
 */

#define MAX_ROMAN 3999 /**< Maximum number for extended range */
#ifndef MAX_NUMBER
    #define MAX_NUMBER 100 /**< The default maximum number */
#endif
#ifndef MIN_NUMBER
    #define MIN_NUMBER 1 /**< The default minimum number */
#endif

#define _(string) gettext(string)

/**
* @brief Converts Roman number to Arabic
*
* @param roman String with Roman number
* @return int Arabic number, or -1 in case of error
* @note Supports numbers from I to MMMCMXCIX (1-3999)
* Does not check for errors in Roman number format
*/
int from_roman(const char *roman) {
    int roman_arabic[7] = {1000, 500, 100, 50, 10, 5, 1};
    char arabic_roman[7] = {'M', 'D', 'C', 'L', 'X', 'V', 'I'};
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
    return res;
}

/**
* @brief Converts Arabic number to Roman
*
* @param roman_s Buffer for result 
* @param len Buffer length
* @param num Arabic number for conversion (1-3999)
* 
* If num >= MAX_ROMAN or len != 16, the function ends without recording
* The buffer must be of sufficient size for the result
*/
void to_roman(char *roman_s, int len, int num) {
    if (len != 16) {
        return;
    }
    if (num >= MAX_ROMAN) {
        return;
    }
    int roman_arabic[7] = {1000, 500, 100, 50, 10, 5, 1};
    char arabic_roman[7] = {'M', 'D', 'C', 'L', 'X', 'V', 'I'};
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
}

struct arguments {
    int roman;
    int extended;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    
    switch (key) {
        case 'r':
            arguments->roman = 1;
            break;
        case 'x':
            arguments->extended = 1;
            break;
            
        case ARGP_KEY_ARG:
            argp_error(state, _("The program does not accept positional arguments"));
            break;
            
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

const char *argp_program_version = PACKAGE_VERSION;

void init_argp(struct argp *argp_ptr) {
    char *doc_ptr;
    static struct argp_option options_ptr[3];
    doc_ptr = _("Numbers guesser.");
    static char args_doc_ptr[] = "";

    _("Usage:");
    _("Give this help list");
    _("Give a short usage message");
    _("Print program version");


    options_ptr[0].name = "roman";
    options_ptr[0].key = 'r';
    options_ptr[0].arg = 0;
    options_ptr[0].flags = 0;
    options_ptr[0].doc = _("Use roman numbers");
    options_ptr[0].group = 0;

    options_ptr[1].name = "extended";
    options_ptr[1].key = 'x';
    options_ptr[1].arg = 0;
    options_ptr[1].flags = 0;
    options_ptr[1].doc = _("Use extended guessing range numbers");
    options_ptr[1].group = 0;
    
    memset(&options_ptr[2], 0, sizeof(struct argp_option));
    
    argp_ptr->options = options_ptr;
    argp_ptr->parser = parse_opt;
    argp_ptr->args_doc = args_doc_ptr;
    argp_ptr->doc = doc_ptr;
    argp_ptr->children = 0;
    argp_ptr->help_filter = 0;
    argp_ptr->argp_domain = 0;
}

void get_string(int use_roman, int num, char *s, int len) {
    if (use_roman) {
        to_roman(s, len, num);
    } else {
        snprintf(s, len, "%d", num);
    }
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALE_PATH);
    textdomain(PACKAGE);

    char str[16];
    struct arguments arguments;
    arguments.roman = 0;
    arguments.extended = 0;
    struct argp argp;
    init_argp(&argp);
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    int low = MIN_NUMBER;
    int high = MAX_NUMBER;
    if (arguments.extended) {
        high = MAX_ROMAN;
    }
    if (arguments.roman && (low < 1 || high > MAX_ROMAN)) {
        printf(_("Error: inappropriate boundaries for using Roman numbers\n"));
        return 1;
    }

    if (low >= high) {
        printf(_("Error: inappropriate boundaries\n"));
        return 1;
    }

    get_string(arguments.roman, low, str, sizeof(str));
    printf(_("Think of a number between %s and "), str);
    get_string(arguments.roman, high, str, sizeof(str));
    printf("%s\n", str);
    
    size_t len = 0;
    char *line = NULL;
    
    while (low < high) {
        int mid = (low + high) / 2;
        get_string(arguments.roman, mid, str, sizeof(str));
        printf(_("Is your number greater than %s? (Yes/No)\n"), str);
        
        while (1) {
            if (getline(&line, &len, stdin) == -1) {
                free(line);
                line = NULL;
                fprintf(stderr, _("Error: failed getline\n"));
                return 1;
            }
            
            line[strlen(line) - 1] = '\0';
            
            if (strcasecmp(line, _("Yes")) == 0 || strcasecmp(line, _("Y")) == 0 || strcmp(line, _("yes")) == 0 || strcmp(line, _("y")) == 0 ) {
                low = mid + 1;
                break;
            } else if (strcasecmp(line, _("No")) == 0 || strcasecmp(line, _("N")) == 0 || strcmp(line, _("no")) == 0 || strcmp(line, _("n")) == 0 ) {
                high = mid;
                break;
            } else {
                printf(_("Error: please answer 'Yes' or 'No'\n"));
            }
        }
    }
    
    get_string(arguments.roman, low, str, sizeof(str));
    printf(_("Your number is %s!\n"), str);
    return 0;
}