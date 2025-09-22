#include <curses.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define DX 7
#define DY 3

int main(int argc, char *argv[]) { //uihpiubpiuobpppppppppppppppppppppppppppppgyictyvs
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
                fclose(file);
                exit(1);
        }
        noecho();
        cbreak();

        frame = newwin(LINES - 2*DY, COLS - 2*DX, DY, DX);
        box(frame, 0, 0);
        mvwaddstr(frame, 0, DX, argv[1]);
        wrefresh(frame);

        win = newwin(LINES - 2*DY - 2, COLS - 2*DX-2, DY+1, DX+1);
        keypad(win, TRUE);
        scrollok(win, FALSE);
        char buf[COLS - 2*DX-1];
        memset(buf, ' ', COLS - 2*DX-3);
        buf[COLS - 2*DX-3] = '\n';
        buf[COLS - 2*DX-2] = 0;
        char *line = NULL;
        size_t size = 0;
        int read;
        int arr_size = 1;
        int *arr = NULL;
        size_t file_size = 0;
        while (1) {
                if ((read = getline(&line, &size, file)) == -1) {
                        break;
                }
                arr = realloc(arr, sizeof(int) * arr_size);
                arr[arr_size - 1] = file_size;
                file_size += read;
                if (read > COLS - 2*DX-2) {
                        line[COLS - 2*DX-3] = '\n';
                        line[COLS - 2*DX-2] = 0;
                }
                if (arr_size < LINES - 2*DY - 1) {
                        wprintw(win, "%s", line);
                }
                arr_size++;
        }
        int lines = 0;
        int rows = 0;
        while((c = wgetch(win)) != 27) {
                wmove(win, 0, 0);
                if (c == ' ' || c == KEY_DOWN) {
                        lines++;
                }
                if (c == KEY_UP) {
                        if (lines > 0) {
                                lines--;
                        }
                }
                if (c == KEY_RIGHT) {
                        rows++;
                }
                if (c == KEY_LEFT) {
                        if (rows > 0) {
                                rows--;
                        }
                }
                if (lines < arr_size - 1) {
                        fseek(file, arr[lines], SEEK_SET);
                }
                for(int i = 0; i < LINES - 2*DY - 2; i++) {
                        if (lines < arr_size - 1) {
                                read = getline(&line, &size, file);
                                if (read > rows) {
                                        size_t len = COLS - 2*DX-1 < read +1 - rows ? COLS - 2*DX-1 : read + 1- rows;
                                        memcpy(buf, line + rows, len);
                                }
                                if (buf[COLS - 2*DX-3] != 0) {
                                        buf[COLS - 2*DX-3] = '\n';
                                        buf[COLS - 2*DX-2] = 0;
                                }
                        }
                        wprintw(win, "%s", buf);
                        memset(buf, ' ', COLS - 2*DX-3);
                        buf[COLS - 2*DX-3] = '\n';
                        buf[COLS - 2*DX-2] = 0;
                }

        }
        delwin(win);
        delwin(frame);
        endwin();
        fclose(file);
        free(line);
        free(arr);
        return 0;
}