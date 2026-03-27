#include <ncurses.h>
#include <unistd.h>

#include "Episodes/EP1/novel_scene.h"
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

  // show_cutscene(game_ctx.wind[2], CHAR_H, 1);
  // show_cutscene(game_ctx.wind[2], CHAR_T, 2);
  // werase(game_ctx.wind[2]);

  while (game_ctx.game_running) {
      getmaxyx(stdscr, y, x);

      if (x < 40 || y < 40) {
        warn_term_size(y, x);
        continue;
      }

      c = getch();
      if (game_ctx.state == STATE_GAME) {
        // 2. input handle
        if (handle_control(c, &game_ctx.player, &game_ctx.map) == 1) break;
      }

      // 3. ncurses drawing
      render_game(game_ctx.wind[0], game_ctx.wind[1], game_ctx.wind[2], game_ctx.ep, &game_ctx.player, frame, &game_ctx.map);

      // 4. update window
      wnoutrefresh(stdscr);
      for (int i=0; i<3; i++) wnoutrefresh(game_ctx.wind[i]);
      doupdate();

      usleep(16000);  /* 60fps */

      update_T_animation(&anim_tick, &frame);
  }

  ncurses_mode(0);
  return 0;
}
