#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>

void ncurses_mode(int mode);
WINDOW * init_window_now(int hei, int wid, int y, int x);

void warn_term_size(int y, int x);

#endif
