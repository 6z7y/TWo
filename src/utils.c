#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>

// mode 1=ON, 0=OFF
void ncurses_mode(int mode)
{
  if (mode) {
    setlocale(LC_ALL, "");

    putenv("TERM=xterm-256color"); /* force before ncurses reads $TERM */

    initscr();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    cbreak();
    keypad(stdscr, TRUE);
    start_color();

    if (can_change_color()) {
      init_color(COLOR_BLACK,      0,    0,    0);
      init_color(COLOR_RED,     1000,    0,    0);
      init_color(COLOR_GREEN,      0, 1000,    0);
      init_color(COLOR_YELLOW,  1000, 1000,    0);
      init_color(COLOR_BLUE,       0,    0, 1000);
      init_color(COLOR_MAGENTA, 1000,    0, 1000);
      init_color(COLOR_CYAN,       0, 1000, 1000);
      init_color(COLOR_WHITE,   1000, 1000, 1000);
    }

    assume_default_colors(COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_RED,     COLOR_BLACK);
    init_pair(2, COLOR_YELLOW,  COLOR_BLACK);
    init_pair(3, COLOR_GREEN,   COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_CYAN,    COLOR_BLACK);
    init_pair(6, COLOR_WHITE,   COLOR_BLACK);
  }
  else {
    endwin();
  }
}

WINDOW *init_window_now(int hei, int wid, int y, int x)
{
  return newwin(hei, wid, y, x);
}

void warn_term_size(int y, int x)
{
  clear();
  mvprintw(y / 2, (x - 19) / 2, "big the term please");
  refresh();
  getch();
}
