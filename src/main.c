#include <ncurses.h>
#include <unistd.h>
#include "Episodes/include_helper.h"
#include "GAME_DATA.h"
#include "control.h"
// #include "Episodes/include_helper.h"
#include "menu.h"
#include "utils.h"
#include "drwing.h"

void setup_game(GAME_Context *game_ctx)
{
  // game on
  game_ctx->game_running = 1;

  // init window
  game_ctx->w[0] = newwin(WG_HEIGHT, WG_WIDTH, WG_Y, WG_X);
  game_ctx->w[1] = newwin(WI_HEIGHT, WI_WIDTH, WI_Y, WI_X);

  // episode select
  // 3. select episode
  game_ctx->ep = select_episode();

  // load map FIRST, then setup player
  load_episode1(&game_ctx->map);
  setup_user_for_ep(&game_ctx->player, &game_ctx->ep, &game_ctx->map);
  for (int i = 0; i < 5; i++) game_ctx->player.inventory[i] = '.';
}

GAME_Context game_ctx = {0};

int main(void) 
{
    int c; // key
    int y, x; // position
    int frame     = 0; // for animation T
    int anim_tick = 0;

    // 1 start game
    ncurses_mode(1); // init ncurses
                     //
    // 2. setup game
    setup_game(&game_ctx);

    while (game_ctx.game_running) {
        getmaxyx(stdscr, y, x);

        // 5. check terminal size
        if (x < 40 || y < 40) {
            warn_term_size(y, x);
            continue;
        }

        // 6. control
        c = getch();
        if (handle_control(c, &game_ctx.player, &game_ctx.map) == 1) break;

        // 7. drawing
        render_game(game_ctx.w[0], game_ctx.w[1], game_ctx.ep, &game_ctx.player, frame, &game_ctx.map);

        // 8. refreshing
        wnoutrefresh(stdscr);
        wnoutrefresh(game_ctx.w[0]);
        wnoutrefresh(game_ctx.w[1]);
        doupdate();

        usleep(16000);  /* 60fps */

        update_T_animation(&anim_tick, &frame);
    }

    ncurses_mode(0);
    return 0;
}
