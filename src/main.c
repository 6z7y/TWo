#include <ncurses.h>
#include <unistd.h>

#include "GAME_DATA.h"
#include "control.h"
#include "drwing.h"
#include "noval_visual.h"
#include "menu.h"
#include "utils.h"

GAME_Context game_ctx = {0};

int menu_section();

int main(void) 
{
  int c; // key
  int y, x; // position

  ncurses_mode(1); // init ncurses

  menu_section();

  
  // 1. setup game
  setup_game(&game_ctx);

  // // if (game_ctx.ep == EP1_KIDNAPPING)
  //     ep1_play_opening(game_ctx.wind[0], game_ctx.wind[1],
  //                      &game_ctx.player, &game_ctx.map);
  // game_ctx.state = STATE_GAME;

  while (game_ctx.game_running) {
      getmaxyx(stdscr, y, x);

      if (x < 40 || y < 40) {
        warn_term_size(y, x);
        continue;
      }

      c = getch();

      // 2. input handle
      if (handle_control(c, &game_ctx.player, &game_ctx.map) == 1) break;

      // // ── check triggers after every move ──
      // if (game_ctx.ep == EP1_KIDNAPPING) {
      //   ep1_check_triggers(
      //       game_ctx.wind[0], game_ctx.wind[1],
      //       &game_ctx.player, &game_ctx.map
      //   );
      // } else if (game_ctx.ep == EP2_PRISON_BREAK) {
      //
      // } else if (game_ctx.ep == EP3_BOSS_FIGHT) {
      //
      // } else if (game_ctx.ep == EP4_HAPPY_ENDING) {

      // }

      // 3. ncurses drawing
      render_game(game_ctx.wind[0], game_ctx.wind[2], game_ctx.ep, &game_ctx.player, &game_ctx.map);

      // 4. update window
      wnoutrefresh(stdscr);
      for (int i=0; i<NUM_WINDOW; i++) wnoutrefresh(game_ctx.wind[i]);
      doupdate();

      usleep(16000);  /* 60fps */
  }

  ncurses_mode(0);
  return 0;
}
