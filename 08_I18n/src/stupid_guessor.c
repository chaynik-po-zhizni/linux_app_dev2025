#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libgen.h>
#include <libintl.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define _(string) gettext(string)

int main() {
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALE_PATH);
    textdomain(PACKAGE);

    printf(_("Think of a number between 1 and 100\n"));
    
    int low = 1;
    int high = 100;
    size_t len = 0;
    char *line = NULL;
    
    
    while (low < high) {
        int mid = (low + high) / 2;
        printf(_("Is your number greater than %d? (Yes/No)\n"), mid);
        
        while (1) {
            if (getline(&line, &len, stdin) == -1) {
                free(line);
                line = NULL;
                fprintf(stderr, _("Error: failed getline\n"));
                return 1;
            }
            
            // Удаляем символ новой строки
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
    
    printf(_("Your number is %d!\n"), low);
    return 0;
}