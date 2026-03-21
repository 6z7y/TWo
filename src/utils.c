#include <stdio.h>
#include <ncurses.h>
#include <locale.h>

// mode 1=ON, 0=OFF
void ncurses_mode(int mode)
{
  if (mode) {
    initscr(); // init ncurses
    curs_set(0); // hide cursor
    noecho(); // no print input
    nodelay(stdscr, TRUE);
    cbreak(); // no buffer
    keypad(stdscr, TRUE); // allow key arrow
    setlocale(LC_ALL, ""); // for support utf-8
    start_color(); // color support
    use_default_colors();
    init_pair(1, COLOR_RED, COLOR_BLACK);     // Walls
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // Doors
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   // Items
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK); // Potions
    init_pair(5, COLOR_CYAN, COLOR_BLACK);    // Exit
    init_pair(6, COLOR_WHITE, COLOR_BLACK);    // Player
  }
  else {
    endwin();
  }
}

WINDOW * init_window_now(int hei, int wid, int y, int x)
{
  return newwin(hei, wid, y, x);
}

void warn_term_size(int *y, int *x) 
{
  clear();
  mvprintw(*y /2, (*x - 19) / 2, "big the term please");
  refresh();
  getch();
}
