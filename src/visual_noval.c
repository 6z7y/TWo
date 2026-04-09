#include <ncurses.h>
#include <unistd.h>   // for usleep

#include "GAME_DATA.h"

// drawing art
#define y_d 2
#define x_d 2

void slot_machine_game();
void tic_tac_toe_game();

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

static inline void draw_G_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, "  #####), ");
    mvwprintw(w, y_d+1, x_d, "  #'o o ) ");
    mvwprintw(w, y_d+2, x_d, "  #   ))) ");
    mvwprintw(w, y_d+3, x_d, "  #-----# ");
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
    mvwprintw(w, y_d+0, x_d, " |      | ");
    mvwprintw(w, y_d+1, x_d, " |'o o  | ");
    mvwprintw(w, y_d+2, x_d, " |||||||| ");
    mvwprintw(w, y_d+3, x_d, "    |     ");
    mvwprintw(w, y_d+4, x_d, "   / \\   ");
    mvwprintw(w, y_d+5, x_d, "          ");
}
static inline void draw_W_art(WINDOW *w) {
    mvwprintw(w, y_d+0, x_d, "+++++++++");
    mvwprintw(w, y_d+1, x_d, "| o | o,|");
    mvwprintw(w, y_d+2, x_d, "+-------+");
    mvwprintw(w, y_d+3, x_d, "    |     ");
    mvwprintw(w, y_d+4, x_d, "   / \\   ");
    mvwprintw(w, y_d+5, x_d, "          ");
}

static inline const char *get_name(Noval_Character who) {
    switch (who) {
        case VN_CHAR_T: return "T";
        case VN_CHAR_P: return "P";
        case VN_CHAR_G: return "G";
        case VN_CHAR_S: return "S";
        case VN_CHAR_U: return "U";
        case VN_CHAR_W: return "W";
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
        case VN_CHAR_T: draw_T_art(wind); break;
        case VN_CHAR_P: draw_P_art(wind); break;
        case VN_CHAR_G: draw_G_art(wind); break;
        case VN_CHAR_S: draw_S_art(wind); break;
        case VN_CHAR_U: draw_U_art(wind); break;
        case VN_CHAR_W: draw_W_art(wind); break;
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

      show_noval_visual(VN_CHAR_T, "...");
      show_noval_visual(VN_CHAR_T, "where am i?");
      show_noval_visual(VN_CHAR_T, "where is [W]?");

      show_noval_visual(VN_CHAR_P, "maybe with [U]");
      show_noval_visual(VN_CHAR_T, "who is [U]?");
      show_noval_visual(VN_CHAR_P, "The boss of criminal");

      show_noval_visual(VN_CHAR_P, "also this is the secret forest. you can't escape.");
      show_noval_visual(VN_CHAR_P, "unless you win my game.");
      show_noval_visual(VN_CHAR_T, "what game?");

      show_noval_visual(VN_CHAR_P, "a slot machine. get three '$' to win.");
      show_noval_visual(VN_CHAR_P, "unlimited tries. your only choice.");

      show_noval_visual(VN_CHAR_T, "ok. i'll try.");

      slot_machine_game();

      show_noval_visual(VN_CHAR_P, "... you won");
      game_ctx.map.layout[29][9] = ' '; // open door
      show_noval_visual(VN_CHAR_P, "the door is open now.");
      show_noval_visual(VN_CHAR_P, "there an pizza eat it");
      show_noval_visual(VN_CHAR_P, "get [W] from [U]! and quit");
      show_noval_visual(VN_CHAR_T, "thanks for your help me.");
    }
  }

  else if (game_ctx.ep == EP2) {

    if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && !ep2_scene1_fired) {
      ep2_scene1_fired = 1;
      show_noval_visual(VN_CHAR_T, "hello");
      show_noval_visual(VN_CHAR_G, "hello");
      show_noval_visual(VN_CHAR_T, "i want see your boss [U]");
      show_noval_visual(VN_CHAR_G, "ohh i see, i have a cake, if u want");
      show_noval_visual(VN_CHAR_G, "u must have power for fight him");
      show_noval_visual(VN_CHAR_T, "sure");
      game_ctx.map.layout[9][4] = '*';
      show_noval_visual(VN_CHAR_G, "take it and eat it, then go");
      game_ctx.player.y = 9;
      game_ctx.player.x = 9;
    }

    else if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && ep2_scene1_fired == 1 && game_ctx.player.health == 1) {
      ep2_scene1_fired = 2;
      show_noval_visual(VN_CHAR_T, "what is this? why i can't control right?");
      show_noval_visual(VN_CHAR_G, "HAHAH!!!! this is a spoiled cake, enjoy");
      show_noval_visual(VN_CHAR_G, "if you want real food u must do something");
      game_ctx.map.layout[28][34] = '@';
      game_ctx.map.layout[8][19] = '@';
      game_ctx.map.layout[14][19] = '@';
      show_noval_visual(VN_CHAR_G, "there a 3 rooks");
      show_noval_visual(VN_CHAR_G, "i want from you push 3 rocks into the holes '_'");
      show_noval_visual(VN_CHAR_T, "but why u do it this?");
      show_noval_visual(VN_CHAR_G, "you have 2 minutes or you die, look in right window");
      game_ctx.timer = 1;
      game_ctx.start = time(NULL);
      game_ctx.player.y = 6;
      game_ctx.player.x = 4;
    }

    else if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && game_ctx.rocks == 3 && ep2_scene1_fired == 2) {
      ep2_scene1_fired = 3;
      show_noval_visual(VN_CHAR_G, "well done you win");
      show_noval_visual(VN_CHAR_G, "take the real food over there");
      game_ctx.map.layout[27][14] = '%';
      show_noval_visual(VN_CHAR_G, "when you eat it u will be fine");
      show_noval_visual(VN_CHAR_G, "u will find [U] you must win for get [W]");
      show_noval_visual(VN_CHAR_T, ".., i will don't tell me what i do");
      show_noval_visual(VN_CHAR_T, "see ya");
      game_ctx.map.layout[29][9] = ' '; // open door
    }
  }

  else if (game_ctx.ep == EP3) {

    if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && !ep3_scene1_fired) {
      ep3_scene1_fired = 1;
      show_noval_visual(VN_CHAR_T, "hello old man");
      show_noval_visual(VN_CHAR_S, "hello");
      show_noval_visual(VN_CHAR_T, "i want come to [U] if you know where him");
      show_noval_visual(VN_CHAR_S, "he's further ahead, and she is with him");
      show_noval_visual(VN_CHAR_T, "her? u mean [W]?");
      show_noval_visual(VN_CHAR_S, "yeah i mean it");
      show_noval_visual(VN_CHAR_S, "if u want quit from here u must get my keys");
      show_noval_visual(VN_CHAR_S, "find my keys, they're hidden under 'x'");
      show_noval_visual(VN_CHAR_S, "i'll give u my Bulldozer '^', and find it from 'x'");
      show_noval_visual(VN_CHAR_T, "i'll find the key");
      game_ctx.map.layout[3][6] = '^';
    }

    else if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && game_ctx.player.inventory[1] == '&') {
      show_noval_visual(VN_CHAR_T, "i found it the keys");
      show_noval_visual(VN_CHAR_S, "ohh nice nice u can quit from here thank you");
      game_ctx.player.inventory[0] = ' ';
      game_ctx.player.inventory[1] = ' ';
      game_ctx.map.layout[29][9] = ' '; // open door
    }
  }

  else if (game_ctx.ep == EP4) {

    if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && !ep4_scene1_fired) {
      ep4_scene1_fired = 1;
      show_noval_visual(VN_CHAR_U, "hello [T]");
      show_noval_visual(VN_CHAR_T, "[U] ..!");
      show_noval_visual(VN_CHAR_W, "[T] help me!!");
      show_noval_visual(VN_CHAR_T, "why you do this [U] answer!");
      show_noval_visual(VN_CHAR_U, "hh, you forget?");
      show_noval_visual(VN_CHAR_T, "forget what?");
      show_noval_visual(VN_CHAR_U, "anyway i want play with u game");
      show_noval_visual(VN_CHAR_U, "if you win, you leave with [W]. but if i win i can kill u");
      show_noval_visual(VN_CHAR_T, "what? what is this game make me played for life or died?");
      show_noval_visual(VN_CHAR_U, "Tic-Tac-Toe (X/O)");
      show_noval_visual(VN_CHAR_U, "you have 3 tryes or i will kill u");
      show_noval_visual(VN_CHAR_U, "are u ready? for died =)");
      show_noval_visual(VN_CHAR_T, "i ready");
      tic_tac_toe_game();
      show_noval_visual(VN_CHAR_U, "good job [T]");
      show_noval_visual(VN_CHAR_U, "take [W] and leave");
      show_noval_visual(VN_CHAR_T, "but your not anser me, why you take her?");
      show_noval_visual(VN_CHAR_U, "I'm just evil. that's what bad people do.");
      show_noval_visual(VN_CHAR_T, "..");
      game_ctx.map.layout[11][60] = ' ';
      game_ctx.map.layout[29][9] = ' '; // open door
    }

    if (game_ctx.player.y == 11 && game_ctx.player.x == 61 && game_ctx.map.layout[11][62] == 'W') {
      show_noval_visual(VN_CHAR_T, "[W]! we must get are there now!");
      show_noval_visual(VN_CHAR_T, "hm");
      game_ctx.map.layout[11][62] = ' ';
      game_ctx.map.layout[30][5] = 'W';
    }
  }
}
