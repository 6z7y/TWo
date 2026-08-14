#include <ncurses.h>

#define HEIGHT 10
#define WIDTH 30

static const char *choices[] = {
  "Start Game",
  "Options",
  "Exit"
};

int menu_section()
{
    int n_choices = 3;
    int highlight = 0;
    int ch;

    int starty = (LINES - HEIGHT) / 2;
    int startx = (COLS - WIDTH) / 2;

    WINDOW *menu_win = newwin(HEIGHT, WIDTH, starty, startx);

    while (1) {
        werase(menu_win);
        box(menu_win, 0, 0);

        // Title
        mvwprintw(menu_win, 1, (WIDTH - 10)/2, "MAIN MENU");

        for (int i = 0; i < n_choices; i++) {
            int y = 3 + i * 2; // spacing between items

            if (i == highlight) {
                wattron(menu_win, A_BOLD | A_REVERSE);
            }

            mvwprintw(menu_win, y, (WIDTH - 12)/2, "%s", choices[i]);

            wattroff(menu_win, A_BOLD | A_REVERSE);
        }

        wrefresh(menu_win);

        ch = getch();

        if (ch == KEY_UP) {
            highlight--;
            if (highlight < 0) highlight = n_choices - 1;
        } 
        else if (ch == KEY_DOWN) {
            highlight++;
            if (highlight >= n_choices) highlight = 0;
        } 
        else if (ch == '\n') {
            break;
        }
    }

    werase(menu_win);
    box(menu_win, 0, 0);
    mvwprintw(menu_win, HEIGHT/2, (WIDTH - 20)/2,
              "Selected: %s", choices[highlight]);
    wrefresh(menu_win);

    getch();
    return 0;
}
