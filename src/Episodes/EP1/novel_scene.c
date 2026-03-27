#include <ncurses.h>

#include "novel_scene.h"

/* Player portrait (shown top-left) */
/* ── T ── tall serious fighter */
static inline void draw_T_art(int y, int x) {
    mvprintw(y, x, "  +-----+  ");
    mvprintw(y, x, "  | - - |  ");
    mvprintw(y, x, "  |  >  |  ");
    mvprintw(y, x, "  +--+--+  ");
    mvprintw(y, x, "   / | \\  ");
    mvprintw(y, x, "  [_] [_]  ");
}

/* ── H ── EP1 Boss, heavy armored */
static inline void draw_H_art(int y, int x) {
    mvprintw(y, x, " /=======\\ ");
    mvprintw(y, x, " |# . . #| ");
    mvprintw(y, x, " |# ___ #| ");
    mvprintw(y, x, " \\==+==+/ ");
    mvprintw(y, x, "   /| |\\  ");
    mvprintw(y, x, "  [=] [=] ");
}

/* ── W ── wide strong character */
static inline void draw_W_art(int y, int x) {
  mvprintw(y, x, " +-------+ ");
  mvprintw(y, x, " | o   o | ");
  mvprintw(y, x, " |  \\_/  | ");
  mvprintw(y, x, " +---+---+ ");
  mvprintw(y, x, "  /  |  \\ ");
  mvprintw(y, x, " /   |   \\");
}

static inline void draw_F_art(int y, int x) {
    mvprintw(y, x, "  +-----+--");
    mvprintw(y, x, "  | - - |  ");
    mvprintw(y, x, "  |  >  |--");
    mvprintw(y, x, "  +--+--+  ");
    mvprintw(y, x, "   / |     ");
    mvprintw(y, x, "  [_]      ");
}
// /* ── O ── small round NPC / support */
// static inline void draw_O_art(int y, int x) {
//   mvprintw(y, x, "  +-----+  ");
//   mvprintw(y, x, "  | o o |  ");
//   mvprintw(y, x, "  | ~~~ |  ");
//   mvprintw(y, x, "  +--+--+  ");
//   mvprintw(y, x, "     |     ");
//   mvprintw(y, x, "    / \\   ");
// }


const char *get_message(int msg_id) {
    switch (msg_id) {
        case 0:  return "...";
        case 1:  return "You dare challenge me?!";
        case 2:  return "I have come to stop you!";
        case 3:  return "This is not over yet!";
        case 4:  return "You will regret this!";
        case 10: return "Welcome, hero. Your journey begins.";
        case 11: return "The kingdom needs you. Will you help?";
        default: return "[unknown message]";
    }
}

static inline const char *get_name(int who)
{
  switch(who) {
    case CHAR_T: return "T";
    case CHAR_H: return "H";
    case CHAR_F: return "F";
    case CHAR_W: return "W";
    default: return "?";
  }
}

void show_cutscene(WINDOW *win, Noval_Character who, int msg_id)
{
  switch(who) {
    case CHAR_T: draw_T_art(2, 7); break;
    case CHAR_H: draw_H_art(2, 7); break;
    case CHAR_F: draw_F_art(2, 7); break;
    case CHAR_W: draw_W_art(2, 7); break;
  }

  // name
  wattron(win, A_UNDERLINE);
  mvwprintw(win, 10, 2, "[ %s ]", get_message(who));
  wattroff(win, A_UNDERLINE);

  // ── Zone 3: message ──
  mvwhline(win, 11, 1, ACS_HLINE, WN_WIDTH - 2);  // separator line
  mvwprintw(win, 13, 2, "%s", get_message(msg_id));

  // ── Zone 4: key hint ──
  mvwprintw(win, WN_HEIGHT - 2, 2, "[ Press space ]");

  // ── flush and wait ──
  wrefresh(win);
  while (1) if (wgetch(win) == ' ') break;
}
