#include <ncurses.h>
#include <unistd.h>   // for usleep

#include "GAME_DATA.h"

// drawing art
#define y_d 2
#define x_d 2

void slot_machine_game();

static int ep1_scene1_fired = 0;
static int ep2_scene1_fired = 0;
static int ep3_scene1_fired = 0;
static int ep4_scene1_fired = 0;

static inline void draw_T_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, "  +-----+  ");
    mvwprintw(w, y_d+1, x_d, "  |'o o |  ");
    mvwprintw(w, y_d+2, x_d, "  +-----|  ");
    mvwprintw(w, y_d+3, x_d, "     |     ");
    mvwprintw(w, y_d+4, x_d, "    / \\   ");
    mvwprintw(w, y_d+5, x_d, "           ");
}

static inline void draw_P_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, "  (####   ");
    mvwprintw(w, y_d+1, x_d, "  ('o o)  ");
    mvwprintw(w, y_d+2, x_d, "  (****   ");
    mvwprintw(w, y_d+3, x_d, "  |       ");
    mvwprintw(w, y_d+4, x_d, " / \\     ");
    mvwprintw(w, y_d+5, x_d, "          ");
}

static inline void draw_D_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, "  #####), ");
    mvwprintw(w, y_d+1, x_d, "  #'o o ) ");
    mvwprintw(w, y_d+2, x_d, "  #     ) ");
    mvwprintw(w, y_d+3, x_d, "  #####)  ");
    mvwprintw(w, y_d+4, x_d, "    |     ");
    mvwprintw(w, y_d+5, x_d, "   / \\   ");
}
static inline void draw_S_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, " #)))))#  ");
    mvwprintw(w, y_d+1, x_d, " # 'o o#  ");
    mvwprintw(w, y_d+2, x_d, " #     #  ");
    mvwprintw(w, y_d+3, x_d, " #(((((#  ");
    mvwprintw(w, y_d+4, x_d, "    |     ");
    mvwprintw(w, y_d+5, x_d, "   / \\   ");
}
static inline void draw_U_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, "||      ||");
    mvwprintw(w, y_d+1, x_d, "||'o o  ||");
    mvwprintw(w, y_d+2, x_d, "||||||||||");
    mvwprintw(w, y_d+3, x_d, "    |     ");
    mvwprintw(w, y_d+4, x_d, "   / \\   ");
    mvwprintw(w, y_d+5, x_d, "          ");
}
static inline void draw_W_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, "||    , ||");
    mvwprintw(w, y_d+1, x_d, "|| o o  ||");
    mvwprintw(w, y_d+2, x_d, "||||||||||");
    mvwprintw(w, y_d+3, x_d, "    |     ");
    mvwprintw(w, y_d+4, x_d, "   / \\   ");
    mvwprintw(w, y_d+5, x_d, "          ");
}

static inline const char *get_name(Noval_Character who) {
    switch (who) {
        case NV_CHAR_T: return "T";
        case NV_CHAR_P: return "P";
        case NV_CHAR_D: return "D";
        case NV_CHAR_S: return "S";
        case NV_CHAR_U: return "U";
        case NV_CHAR_W: return "W";
        default:     return "?";
    }
}

// ── show one dialogue line in wind_second ──────────
void show_noval_visual(Noval_Character who, const char *msg)
{
  WINDOW *wind = game_ctx.wind[2];

    werase(wind);
    box(wind, 0, 0);

    // Zone 1: portrait — fix_ded y+row offset, draws into win
    switch (who) {
        case NV_CHAR_T: draw_T_art(wind); break;
        case NV_CHAR_P: draw_P_art(wind); break;
        case NV_CHAR_D: draw_D_art(wind); break;
        case NV_CHAR_S: draw_S_art(wind); break;
        case NV_CHAR_U: draw_U_art(wind); break;
    }

    // Zone 2: speaker name — get_name not get_message!
    wattron(wind, A_BOLD);
    mvwprintw(wind, 0, 1, " [ %s ]", get_name(who));
    wattroff(wind, A_BOLD);

    // Zone 3: separator + message
    mvwprintw(wind, 2, 12, "%s", msg);

    // Zone 4: hint
    mvwprintw(wind, WN_HEIGHT - 1, WN_WIDTH - 16, " SPACE = next ");

    wrefresh(wind);

    // block until SPACE — user reads, then continues
    nodelay(stdscr, FALSE);          // blocking mode
    while (wgetch(wind) != ' ');
    nodelay(stdscr, TRUE);           // back to non-blocking
}

void visual_noval_checker()
{
  if (game_ctx.ep == EP1) {
    if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && !ep1_scene1_fired) {
      ep1_scene1_fired = 1;

      show_noval_visual(NV_CHAR_T, "...");
      show_noval_visual(NV_CHAR_T, "where am i?");
      show_noval_visual(NV_CHAR_T, "where [W]?");

      show_noval_visual(NV_CHAR_P, "maybe with [U]");
      show_noval_visual(NV_CHAR_T, "who that [U]?");
      show_noval_visual(NV_CHAR_P, "The boss of criminal");

      show_noval_visual(NV_CHAR_P, "also this is the secret forest. you can't escape.");
      show_noval_visual(NV_CHAR_P, "unless you win my game.");
      show_noval_visual(NV_CHAR_T, "what game?");

      show_noval_visual(NV_CHAR_P, "a slot machine. get three '$' to win.");
      show_noval_visual(NV_CHAR_P, "unlimited tries. your only choice.");

      show_noval_visual(NV_CHAR_T, "ok. i'll try.");

      slot_machine_game();

      show_noval_visual(NV_CHAR_P, "... you won");
      game_ctx.map.layout[29][9] = ' '; // open door
      show_noval_visual(NV_CHAR_P, "the door is open now.");
      show_noval_visual(NV_CHAR_P, "there an pizza eat it");
      show_noval_visual(NV_CHAR_P, "get [W] from [U]! and quit");
      show_noval_visual(NV_CHAR_T, "thanks for your help me.");
    }
  }

  else if (game_ctx.ep == EP2) {

    if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && !ep2_scene1_fired) {
      ep2_scene1_fired = 1;
      show_noval_visual(NV_CHAR_T, "hello");
      show_noval_visual(NV_CHAR_D, "hello");
      show_noval_visual(NV_CHAR_T, "i want see your boss [U]");
      show_noval_visual(NV_CHAR_D, "ohh i see, i have a cake, if u want");
      show_noval_visual(NV_CHAR_D, "u must have power for fight him");
      show_noval_visual(NV_CHAR_T, "sure");
      game_ctx.map.layout[9][4] = '*';
      show_noval_visual(NV_CHAR_D, "take it and eat it, then go");
      game_ctx.player.y = 9;
      game_ctx.player.x = 9;
    }

    else if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && ep2_scene1_fired == 1 && game_ctx.player.health == 1) {
      ep2_scene1_fired = 2;
      show_noval_visual(NV_CHAR_T, "what is this? why i can't control right?");
      show_noval_visual(NV_CHAR_D, "HAHAH!!!! this is a spoiled cake");
      show_noval_visual(NV_CHAR_D, "if u want real food u must do something");
      game_ctx.map.layout[28][34] = '@';
      game_ctx.map.layout[8][19] = '@';
      game_ctx.map.layout[14][19] = '@';
      show_noval_visual(NV_CHAR_D, "there a 3 rooks i want from u Excavations cover it's '_'");
      show_noval_visual(NV_CHAR_T, "but why u do it this?");
      show_noval_visual(NV_CHAR_D, "oh yes, u have 2min or u will die");
      game_ctx.timer = 1;
      game_ctx.start = time(NULL);
      game_ctx.player.y = 6;
      game_ctx.player.x = 4;
    }

    else if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && game_ctx.rocks == 3) {
      show_noval_visual(NV_CHAR_D, "well done u win");
      show_noval_visual(NV_CHAR_D, "take a real food from there");
      game_ctx.map.layout[27][14] = '%';
      show_noval_visual(NV_CHAR_D, "when you eat it u will be fine");
      show_noval_visual(NV_CHAR_D, "u will find [U] you must win for get [W]");
      show_noval_visual(NV_CHAR_T, ".., i will don't tell me what i do");
      show_noval_visual(NV_CHAR_T, "see ya");
      game_ctx.map.layout[29][9] = ' '; // open door
    }

    // else if (game_ctx)
  }

  else if (game_ctx.ep == EP3) {

    // if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && !ep1_scene1_fired) {
    //   ep1_scene1_fired = 1;
    //   show_noval_visual(NV_CHAR_H, 3);
    //   show_noval_visual(NV_CHAR_U, 3);
    // }
  }

  else if (game_ctx.ep == EP4) {

    // if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && !ep1_scene1_fired) {
    //   ep1_scene1_fired = 1;
    //   show_noval_visual(NV_CHAR_H, 3);
    //   show_noval_visual(NV_CHAR_U, 3);
    // }
  }
}
