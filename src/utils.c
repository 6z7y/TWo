#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>

#include "GAME_DATA.h"
#include "visual_noval.h"
#include "save.h"

static inline void setup_user_for_map(Player *player, Define_Episode ep, MAP_Structure *episode_struct)
{
  if (ep == EP1) *player = (Player){ .health = 1, .y = episode_struct->player_start_y, .x = episode_struct->player_start_x };
  if (ep == EP2) *player = (Player){ .health = 2, .y = episode_struct->player_start_y, .x = episode_struct->player_start_x };
  if (ep == EP3) *player = (Player){ .health = 3, .y = episode_struct->player_start_y, .x = episode_struct->player_start_x };
  if (ep == EP4) *player = (Player){ .health = 3, .y = episode_struct->player_start_y, .x = episode_struct->player_start_x };
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
  game_ctx->reload_game = 0;

  game_ctx->rocks = 0;

  game_ctx->wind[0] = newwin(WG_HEIGHT, WG_WIDTH, WG_Y, WG_X); // wind game
  game_ctx->wind[1] = newwin(WS_HEIGHT, WS_WIDTH, WS_Y, WS_X); // wind second
  game_ctx->wind[2] = newwin(WN_HEIGHT, WN_WIDTH, WN_Y, WN_X); // wind noval
  game_ctx->wind[3] = newwin(WIC_HEIGHT, WIC_WIDTH, WIC_Y, WIC_X); // wind inventory

  // load map from a save file
  load_current_episode(&game_ctx->ep);
  setup_user_for_map(&game_ctx->player, game_ctx->ep, &game_ctx->map);
  for (int i = 0; i < 5; i++) game_ctx->player.inventory[i] = ' ';
}

void warn_term_size(int y, int x)
{
  clear();
  mvprintw(y / 2, (x - 19) / 2, "big the term please");
  refresh();
  getch();
}
