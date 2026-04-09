#include <ncurses.h>
#include <unistd.h>

#include "GAME_DATA.h"
#include "control.h"
#include "drwing.h"
#include "visual_noval.h"
#include "menu.h"
#include "utils.h"

GAME_Context game_ctx = {0};

int menu_section();

int main(void) 
{
  int c; // key
  int y, x; // position

  ncurses_mode(1); // init ncurses

  // menu_section();
  
  // 1. setup game
reinit:
  setup_game(&game_ctx);

  while (game_ctx.game_running) {
      getmaxyx(stdscr, y, x);

      if (x < 40 || y < 40) {
        warn_term_size(y, x);
        continue;
      }

      c = getch();

      // 2. input handle
      if (handle_control(c, &game_ctx.player, &game_ctx.map) == 1) break;

      // 3. ncurses drawing
      render_game(game_ctx.ep, &game_ctx.player, &game_ctx.map);

      // 4. update window
      wnoutrefresh(stdscr);
      for (int i=0; i<NUM_WINDOW; i++) wnoutrefresh(game_ctx.wind[i]);
      doupdate();

      if (game_ctx.reload_game) goto reinit;

      usleep(16000);  /* 60fps */
  }

  ncurses_mode(0);
  return 0;
}
