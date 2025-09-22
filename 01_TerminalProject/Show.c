#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define DX 7
#define DY 3

int main(int argc, char *argv[]) {
        int res = 0;
        WINDOW *frame, *win;
        int c = 0;
        if (argc != 2) {
                fprintf(stderr, "Wrong number of arguments.\n");
                exit(1);
        }
        FILE *file;
        file = fopen(argv[1], "r");
        if (!file) {
                fprintf(stderr, "Error open file.\n");
                exit(1);
        }

        setlocale(LC_ALL, "");
        if (!initscr()) {
                fprintf(stderr, "Error initialising ncurses.\n");
                res = 1;
                goto ERR_FILE;
        }
        noecho();
        cbreak();

        frame = newwin(LINES - 2*DY, COLS - 2*DX, DY, DX);
        box(frame, 0, 0);
        mvwaddstr(frame, 0, DX, argv[1]);
        wrefresh(frame);

        win = newwin(LINES - 2*DY - 2, COLS - 2*DX-2, DY+1, DX+1);
        keypad(win, TRUE);
        scrollok(win, TRUE);
        char *line = NULL;
        size_t size = 0;
        int read;
        int i = 0;
        while (1) {
                if ((read = getline(&line, &size, file)) == -1) {
                        break;
                }
                if (read > COLS - 2*DX-2) {
                        line[COLS - 2*DX-3] = '\n';
                        line[COLS - 2*DX-2] = 0;
                }
                wprintw(win, "%s", line);
                i++;
                if (i >= LINES - 2*DY - 3) {
                        break;
                }
        }
        while((c = wgetch(win)) != 27) {
                if (c == ' ') {
                        if ((read = getline(&line, &size, file)) == -1) {
                                scrollok(win, FALSE);
                                continue;
                        }
                        if (read > COLS - 2*DX-2) {
                                line[COLS - 2*DX-3] = '\n';
                                line[COLS - 2*DX-2] = 0;
                        }
                        wprintw(win, "%s", line);
                }
        }
        delwin(win);
        delwin(frame);
        endwin();
ERR_FILE:
        fclose(file);
        free(line);
        return res;
}