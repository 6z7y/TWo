#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>

#include "GAME_DATA.h"
#include "Episodes/include_helper.h"
#include "noval_visual.h"
#include "save.h"
// #include "drwing.h"

static inline void setup_user_for_map(Player *player, Define_Episode *ep, MAP_Structure *episode_struct)
{
  if (*ep == EP1_KIDNAPPING) *player = (Player){ .health = 1, .y = episode_struct->player_start_y, .x = episode_struct->player_start_x };
  if (*ep == EP2_PRISON_BREAK) *player = (Player){ .health = 2, .y = episode_struct->player_start_y, .x = episode_struct->player_start_x };
}


void setup_color();

// mode 1=ON, 0=OFF
void ncurses_mode(int mode)
{
  if (mode) {
    setlocale(LC_ALL, "");

    putenv("TERM=xterm-256color"); /* force before ncurses reads $TERM */

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    setup_color();

  }
  else {
    endwin();
    printf("bye");
  }
}

void setup_game(GAME_Context *game_ctx)
{
    game_ctx->game_running = 1;

    // ── windows FIRST ─────────────────────────────
    game_ctx->wind[0] = newwin(WG_HEIGHT, WG_WIDTH, WG_Y, WG_X);
    game_ctx->wind[1] = newwin(WS_HEIGHT, WS_WIDTH, WS_Y, WS_X);
    game_ctx->wind[2] = newwin(WIC_HEIGHT, WIC_WIDTH, WIC_Y, WIC_X);

    // ── then load map from save ────────────────────
    load_current_episode(&game_ctx->ep);

    setup_user_for_map(&game_ctx->player, &game_ctx->ep, &game_ctx->map);
    for (int i = 0; i < 5; i++) game_ctx->player.inventory[i] = ' ';
    show_noval_visual(NV_CHAR_H, 1);
    show_noval_visual(NV_CHAR_T, 2);
}

void warn_term_size(int y, int x)
{
  clear();
  mvprintw(y / 2, (x - 19) / 2, "big the term please");
  refresh();
  getch();
}
