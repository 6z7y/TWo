#include <ncurses.h>
#include <unistd.h>
#include "Episodes/include_helper.h"
#include "GAME_DATA.h"
#include "control.h"
// #include "Episodes/include_helper.h"
#include "menu.h"
#include "utils.h"
#include "drwing.h"

int main(void) 
{
    int c; // key
    int y, x; // position
    int frame     = 0; // for animation T
    int anim_tick = 0;

    // 1 start game
    GAME_context game_ctx = {.game_running = 1};
    ncurses_mode(1); // init ncurses

    Map episode1_struct;
    load_episode1(&episode1_struct);

    // 3. select episode
    int current_episode = select_episode();

    // 3. setup windowses
    NEW_Wind w_game      = { init_window_now(WG_HEIGHT, WG_WIDTH, WG_Y, WG_X) };
    // NEW_Wind w_status    = { init_window_now(WS_HEIGHT, WS_WIDTH, WS_Y, WS_X) };
    NEW_Wind w_inventory = { init_window_now(WI_HEIGHT, WI_WIDTH, WI_Y, WI_X) };

    // 4. setup user
    Player player = {0};
    setup_user_for_ep(&player, &current_episode, &episode1_struct);
    for (int i=0; i<5; i++) player.inventory[i] = '.';

    while (game_ctx.game_running) {
        getmaxyx(stdscr, y, x);

        // 5. check terminal size
        if (x < 40 || y < 40) {
            warn_term_size(y, x);
            continue;
        }

        // 6. control
        c = getch();
        if (handle_control(c, &player, &episode1_struct) == 1) break;

        // 7. drawing
        render_game(&w_game, &w_inventory, current_episode, &player, frame, &episode1_struct);

        // 8. refreshing
        wnoutrefresh(stdscr);
        wnoutrefresh(w_game.wind);
        wnoutrefresh(w_inventory.wind);
        doupdate();

        usleep(16000);  /* 60fps */

        update_T_animation(&anim_tick, &frame);
    }

    ncurses_mode(0);
    return 0;
}
