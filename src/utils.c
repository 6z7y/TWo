#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>

#include "GAME_DATA.h"
#include "Episodes/include_helper.h"
// #include "drwing.h"

static inline void setup_user_for_map(Player *player, Define_Episode *ep, MAP_Structure *episode1_struct)
{
  if (*ep == EP1_KIDNAPPING)
    *player = (Player){ .health = 1, .y = episode1_struct->player_start_y, .x = episode1_struct->player_start_x };
}

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
    start_color();

    if (can_change_color()) {
      init_color(COLOR_BLACK,      0,    0,    0);
      init_color(COLOR_RED,     1000,    0,    0);
      init_color(COLOR_BLUE,       0,    0, 1000);
      init_color(COLOR_GREEN,      0, 1000,    0);
      init_color(COLOR_YELLOW,  1000, 1000,    0);
      init_color(COLOR_MAGENTA, 1000,    0, 1000);
      init_color(COLOR_CYAN,       0, 1000, 1000);
      init_color(COLOR_WHITE,   1000, 1000, 1000);
    }

    // assume_default_colors(COLOR_WHITE, COLOR_BLACK);

    init_pair(1, COLOR_RED,     COLOR_RED);   // 1: tile
    init_pair(2, COLOR_BLUE,    COLOR_BLACK); // 2: ???
    init_pair(3, COLOR_GREEN,   COLOR_BLACK); // 3: items
    init_pair(4, COLOR_YELLOW,  COLOR_BLACK); // 4: ???
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK); // 5: characters
    init_pair(6, COLOR_CYAN,    COLOR_BLACK); // 6: Exit
    init_pair(7, COLOR_WHITE,   COLOR_BLACK); // 7: other
  }
  else {
    printf("bye");
    endwin();
  }
}

int select_episode()
{
  Define_Episode ep = EP2_PRISON_BREAK;
  return ep;
}

void setup_game(GAME_Context *game_ctx)
{
  // game on
  game_ctx->game_running = 1;

  // init window
  game_ctx->wind[0] = newwin(WG_HEIGHT, WG_WIDTH, WG_Y, WG_X);
  game_ctx->wind[1] = newwin(WIC_HEIGHT, WIC_WIDTH, WIC_Y, WIC_X);
  game_ctx->wind[2] = newwin(WS_HEIGHT, WS_WIDTH, WS_Y, WS_X);

  // episode select
  // 3. select episode
  game_ctx->ep = select_episode();

  // load map FIRST, then setup player
  load_episode1(&game_ctx->map);
  setup_user_for_map(&game_ctx->player, &game_ctx->ep, &game_ctx->map);
  for (int i = 0; i < 5; i++) game_ctx->player.inventory[i] = '.';
}

void warn_term_size(int y, int x)
{
  clear();
  mvprintw(y / 2, (x - 19) / 2, "big the term please");
  refresh();
  getch();
}
