#include <string.h>
#include <ncurses.h>
#include "episodes.h"
#include "GAME_DATA.h"

/* ── episode 1 : Cave of Secrets ────────────────── */
void load_episode1(Map *map)
{
    /* basic info */
    strncpy(map->name, "Cave of Secrets", sizeof(map->name));
    map->player_start_x = 15;
    map->player_start_y = 6;
    map->key_x          = 40;
    map->key_y          = 15;

    /* layout — copy each row */
    strncpy(map->layout[ 0], "########################################################################################", WG_WIDTH);
    strncpy(map->layout[ 1], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[ 2], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[ 3], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[ 4], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[ 5], "#         ||||||||||||||||||                                                           #", WG_WIDTH);
    strncpy(map->layout[ 6], "#         |                |                                                           #", WG_WIDTH);
    strncpy(map->layout[ 7], "#         |                |                                                           #", WG_WIDTH);
    strncpy(map->layout[ 8], "#         |                |                                                           #", WG_WIDTH);
    strncpy(map->layout[ 9], "#                          |                                                           #", WG_WIDTH);
    strncpy(map->layout[10], "#         ||||||||||||||||||                                                           #", WG_WIDTH);
    strncpy(map->layout[11], "#             E                                                                        #", WG_WIDTH);
    strncpy(map->layout[12], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[13], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[14], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[15], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[16], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[17], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[18], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[19], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[20], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[21], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[22], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[23], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[24], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[25], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[26], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[27], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[28], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[29], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[30], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[31], "#                                                                                      #", WG_WIDTH);
    strncpy(map->layout[32], "########################################################################################", WG_WIDTH);

    /* you can now program changes here — e.g. add a trap at runtime:
     *   map->layout[20][30] = TILE_POTION;
     *   map->layout[5][50]  = TILE_KEY;
     */
}

// /* ── episode 2 : placeholder ─────────────────────── */
// void load_episode2(Map *map)
// {
//     strncpy(map->name, "Prison Break", sizeof(map->name));
//     map->player_start_x = 5;
//     map->player_start_y = 5;
//     map->key_x          = 60;
//     map->key_y          = 20;
//
//     /* fill all rows with walls first */
//     for (int i = 0; i < WG_HEIGHT; i++) {
//         for (int j = 0; j < WG_WIDTH; j++)
//             map->layout[i][j] = '#';
//         map->layout[i][WG_WIDTH] = '\0';
//     }
//
//     /* carve out inside */
//     for (int i = 1; i < WG_HEIGHT - 1; i++)
//         for (int j = 1; j < WG_WIDTH - 1; j++)
//             map->layout[i][j] = ' ';
//
//     /* place exit */
//     map->layout[20][70] = TILE_EXIT;
// }
