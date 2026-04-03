#include <ncurses.h>
#include <unistd.h>   // for usleep

#include "GAME_DATA.h"

// drawing art
#define y_d 24
#define x_d 8

static inline void draw_T_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, "  +-----+  ");
    mvwprintw(w, y_d+1, x_d, "  | - - |  ");
    mvwprintw(w, y_d+2, x_d, "  |  >  |  ");
    mvwprintw(w, y_d+3, x_d, "  +--+--+  ");
    mvwprintw(w, y_d+4, x_d, "   / | \\  ");
    mvwprintw(w, y_d+5, x_d, "  [_] [_]  ");
}


static inline void draw_H_art(WINDOW *w) {
    

  mvwprintw(w, y_d+0, x_d, "|____| |____|");
  mvwprintw(w, y_d+0, x_d, "|    | |    |");
  mvwprintw(w, y_d+1, x_d, "| o  | |  o |");
  mvwprintw(w, y_d+2, x_d, "|    |_|    |");
  mvwprintw(w, y_d+3, x_d, "|           |");
  mvwprintw(w, y_d+4, x_d, "|    | |    |");
  mvwprintw(w, y_d+6, x_d, "|____| |____|");
  mvwprintw(w, y_d+6, x_d, "|    | |    |");
  mvwprintw(w, y_d+6, x_d, "|____| |____|");
  mvwprintw(w, y_d+6, x_d, "  \\(     )/  ");
}

static inline void draw_W_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, " +-------+ ");
    mvwprintw(w, y_d+1, x_d, " | o   o | ");
    mvwprintw(w, y_d+2, x_d, " |  \\_/  | ");
    mvwprintw(w, y_d+3, x_d, " +---+---+ ");
    mvwprintw(w, y_d+4, x_d, "  /  |  \\ ");
    mvwprintw(w, y_d+5, x_d, " /   |   \\");
}

static inline void draw_F_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, "  +-----+  ");
    mvwprintw(w, y_d+1, x_d, "  | . - |  ");
    mvwprintw(w, y_d+2, x_d, "  |  ~  |  ");
    mvwprintw(w, y_d+3, x_d, "  +--+--+  ");
    mvwprintw(w, y_d+4, x_d, "   / |     ");
    mvwprintw(w, y_d+5, x_d, "  [_]      ");
}

const char *get_message(int msg_id) {
    switch (msg_id) {
        case 0:  return "...";
        case 1:  return "y_dou dare challenge me?!";
        case 2:  return "I have come to stop y_dou!";
        case 3:  return "This is not over y_det!";
        case 4:  return "y_dou will regret this!";
        case 10: return "Welcome. y_dour journey begins.";
        case 11: return "The kingdom needs y_dou.";
        default: return "[unknown]";
    }
}

static inline const char *get_name(Noval_Character who) {
    switch (who) {
        case NV_CHAR_T: return "T";
        case NV_CHAR_W: return "W";
        case NV_CHAR_H: return "H";
        case NV_CHAR_F: return "F";
        default:     return "?";
    }
}

// ── show one dialogue line in wind_second ──────────
void show_noval_visual(Noval_Character who, int msg_id)
{
  WINDOW *win = game_ctx.wind[1];

    werase(win);
    box(win, 0, 0);

    // Zone 1: portrait — fix_ded y+row offset, draws into win
    switch (who) {
        case NV_CHAR_T: draw_T_art(win); break;
        case NV_CHAR_H: draw_H_art(win); break;
        case NV_CHAR_W: draw_W_art(win); break;
        // case CHAR_F: draw_P_art(win, 2, 8); break;
        // case CHAR_F: draw_F_art(win, 2, 8); break;
    }

    // Zone 2: speaker name — get_name not get_message!
    wattron(win, A_BOLD);
    mvwprintw(win, 29, 1, " [ %s ]", get_name(who));
    wattroff(win, A_BOLD);

    // Zone 3: separator + message
    mvwhline(win, 29, 8, ACS_HLINE, WS_WIDTH - 2);
    mvwprintw(win, 31, 3, "%s", get_message(msg_id));

    // Zone 4: hint
    mvwprintw(win, WS_HEIGHT - 2, 2, "[ SPACE = nex_dt ]");

    wrefresh(win);

    // block until SPACE — user reads, then continues
    nodelay(stdscr, FALSE);          // blocking mode
    while (wgetch(win) != ' ');
    nodelay(stdscr, TRUE);           // back to non-blocking
}
