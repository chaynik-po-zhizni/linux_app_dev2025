#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libgen.h>
#include <libintl.h>
#include <argp.h>

#include "roman.h"


/**
 * @file stupid_guessor.c
 * @brief A number guessing program with Roman numerals support
 * @author Vasilieva Vera
 * @date 2025
 * @version 3.0
 */

#ifndef MAX_NUMBER
    /**
     * @brief The default maximum number 
     */
    #define MAX_NUMBER 100 
#endif

#ifndef MIN_NUMBER
    /**
     * @brief The default minimum number
     */
    #define MIN_NUMBER 1 
#endif

#define _(string) gettext(string)

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