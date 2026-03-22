#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "GAME_DATA.h"
#include "control.h"
#include "episodes.h"
#include "menu.h"
#include "utils.h"
#include "drwing.h"

/* map is filled by load_episode1() in main */
Map episode1_struct;

void draw_map(WINDOW *win, Map *map, Player *player) {
    for (int i = 0; i < WG_HEIGHT - 1; i++) {
        if (strlen(map->layout[i]) > 0) {
            for (int j = 0; j < WG_WIDTH - 2 && j < (int)strlen(map->layout[i]); j++) {
                char tile = map->layout[i][j];

                if      (tile == TILE_WALL)                         wattron(win, COLOR_PAIR(1));
                else if (tile == TILE_ITEM)                         wattron(win, COLOR_PAIR(3)); // green like items
                else if (tile == TILE_EXIT)                         wattron(win, COLOR_PAIR(5));

                mvwaddch(win, i + 1, j + 1, tile);

                wattroff(win, COLOR_PAIR(1) | COLOR_PAIR(2) |
                              COLOR_PAIR(3) | COLOR_PAIR(4) | COLOR_PAIR(5));
            }
        }
    }
    wattron(win, A_BOLD | COLOR_PAIR(6));
    mvwprintw(win, player->y + 1, player->x + 1, "T");
    wattroff(win, A_BOLD | COLOR_PAIR(6));
}


/* CHANGE 2: int frame not int* */
void render_game(NEW_Wind *wind_game, NEW_Wind *wind_status, NEW_Wind *wind_inventory, int episode, Player *player, int frame) {

    werase(stdscr);

    box(stdscr, 0, 0);
    box(wind_game->wind, 0, 0);
    // box(wind_status->wind, 0, 0);
    box(wind_inventory->wind, 0, 0);

    mvprintw(0, 4, " TWo ");
    mvprintw(LINES - 2, 4, "Controls: (Arrow/Vim keys): Move | (+/-): incress/decress health | (Q) = Quit");

    if (episode == 1)
        draw_map(wind_game->wind, &episode1_struct, player);

    mvwprintw(wind_status->wind, 0, 0, "Health: %s", health_ascii(player->health) );

    mvwprintw(wind_inventory->wind, 1, 2, " %c | %c | %c | %c | %c ",
        player->inventory[0], player->inventory[1], player->inventory[2], player->inventory[3], player->inventory[4]);

    draw_two(stdscr, 38, 4, player->health, frame);
}

int select_episode()
{
  Episode ep = EP1_KIDNAPPING;
  return ep;

}

void setup_user_for_ep(Player *player, int *current_episode)
{
  if (*current_episode == EP1_KIDNAPPING)
    *player = (Player){ .health = 1, .y = episode1_struct.player_start_y, .x = episode1_struct.player_start_x };
}

int main(void) 
{
    int c;
    int y, x;
    int frame     = 0;
    int anim_tick = 0;

    // 1 start game
    GAME_context game_ctx = {.game_running = 1};
    ncurses_mode(1); // init ncurses

    // 2. load map data into struct
    load_episode1(&episode1_struct);

    // 3. select episode
    int current_episode = select_episode();

    // 3. setup windowses
    NEW_Wind w_game      = { init_window_now(WG_HEIGHT, WG_WIDTH, WG_Y, WG_X) };
    NEW_Wind w_status    = { init_window_now(WS_HEIGHT, WS_WIDTH, WS_Y, WS_X) };
    NEW_Wind w_inventory = { init_window_now(WI_HEIGHT, WI_WIDTH, WI_Y, WI_X) };

    // 4. setup user
    Player player = {0};
    setup_user_for_ep(&player, &current_episode);
    player.inventory[0] = '.';
    player.inventory[1] = '.';
    player.inventory[2] = '.';
    player.inventory[3] = '.';
    player.inventory[4] = '.';

    while (1) {
        getmaxyx(stdscr, y, x);

        // 5. check terminal size
        if (x < 40 || y < 40) {
            warn_term_size(&y, &x);
            continue;
        }

        // 6. control
        c = getch();
        if (handle_control(c, &player, &episode1_struct) == 1) break;

        // 7. drawing
        render_game(&w_game, &w_status, &w_inventory, current_episode, &player, frame);

        // 8. refreshing
        wnoutrefresh(stdscr);
        wnoutrefresh(w_game.wind);
        wnoutrefresh(w_status.wind);
        wnoutrefresh(w_inventory.wind);
        doupdate();

        usleep(16000);  /* 60fps */

        /* CHANGE 5: simple anim_tick — no double increment bug */
        anim_tick++;
        if (anim_tick >= 24) {       /* 6 x 16ms = ~100ms per frame */
            frame = (frame + 1) % 3;
            anim_tick = 0;
        }
    }
    werase(stdscr);

    ncurses_mode(0);
    return 0;
}
