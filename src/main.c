#include <ncurses.h>
#include <unistd.h>

#include "GAME_DATA.h"
#include "control.h"
#include "drwing.h"
#include "menu.h"
#include "utils.h"

GAME_Context game_ctx = {0};

int main(void) 
{
  int c; // key
  int y, x; // position
  int frame     = 0; // for animation T
  int anim_tick = 0;

  ncurses_mode(1); // init ncurses

  // 1. setup game
  setup_game(&game_ctx);

  while (game_ctx.game_running) {
      getmaxyx(stdscr, y, x);

      if (x < 40 || y < 40) {
        warn_term_size(y, x);
        continue;
      }

      // 2. input handle
      c = getch();
      if (handle_control(c, &game_ctx.player, &game_ctx.map) == 1) break;

      // 4. ncurses drawing
      render_game(game_ctx.w[0], game_ctx.w[1], game_ctx.ep, &game_ctx.player, frame, &game_ctx.map);

      // 4. update window
      wnoutrefresh(stdscr);
      wnoutrefresh(game_ctx.w[0]);
      wnoutrefresh(game_ctx.w[1]);
      doupdate();

      usleep(16000);  /* 60fps */

      update_T_animation(&anim_tick, &frame);
  }

  ncurses_mode(0); // close ncurses
  return 0;
}
