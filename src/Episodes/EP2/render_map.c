#include <string.h>
#include <ncurses.h>

#include "render_map.h"
#include "../../utils.h"

/* ── episode 1 : Cave of Secrets ────────────────── */
void load_episode2(MAP_Structure *map)
{
    /* basic info */
    strncpy(map->name, "Cave of Secrets", sizeof(map->name));
    map->player_start_x = 0;
    map->player_start_y = 0;
    map->exit_x         = 40;
    map->exit_y         = 15;

    /* layout — copy each row */
    strncpy(map->layout[ 0], "                                                |                                     ", WG_WIDTH);
    strncpy(map->layout[ 1], "                                                |                                     ", WG_WIDTH);
    strncpy(map->layout[ 2], "                                                |                                      ", WG_WIDTH);
    strncpy(map->layout[ 3], "                                       |||||||||||||||||||||||||||||||||||             ", WG_WIDTH);
    strncpy(map->layout[ 4], "                                       |                        |_ _|    |             ", WG_WIDTH);
    strncpy(map->layout[ 5], " D                                     |                        |   |    |             ", WG_WIDTH);
    strncpy(map->layout[ 6], "                                       ||||||||||||||||||||||||||   |    |             ", WG_WIDTH);
    strncpy(map->layout[ 7], "                                                                    ||||||             ", WG_WIDTH);
    strncpy(map->layout[ 8], "                                                                        _|             ", WG_WIDTH);
    strncpy(map->layout[ 9], "                                       |||||||||||||||||||||||||||||||||||             ", WG_WIDTH);
    strncpy(map->layout[10], "                                       |                                 |             ", WG_WIDTH);
    strncpy(map->layout[11], "                                       |                                 |             ", WG_WIDTH);
    strncpy(map->layout[12], "                                       |||||||||||||||||||||||||||||||||||             ", WG_WIDTH);
    strncpy(map->layout[13], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[14], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[15], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[16], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[17], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[18], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[19], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[20], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[21], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[22], "                                                  |                      |             ", WG_WIDTH);
    strncpy(map->layout[23], "                                                  |                                    ", WG_WIDTH);
    strncpy(map->layout[24], "                                                  |                                    ", WG_WIDTH);
    strncpy(map->layout[25], "                                                  |                                    ", WG_WIDTH);
    strncpy(map->layout[26], "                                                  |                                    ", WG_WIDTH);
    strncpy(map->layout[27], "                                                  |                                    ", WG_WIDTH);
    strncpy(map->layout[28], "||||||||||                                        |                                    ", WG_WIDTH);
    strncpy(map->layout[29], "         |                                        |                                    ", WG_WIDTH);
    strncpy(map->layout[30], "     %   |                                        |                                    ", WG_WIDTH);
    strncpy(map->layout[31], "         |                                        |                                    ", WG_WIDTH);
    strncpy(map->layout[32], "[EXI ]   |                                        |                                    ", WG_WIDTH);


    /* you can now program changes here — e.g. add a trap at runtime:
     *   map->layout[20][30] = TILE_POTION;
     *   map->layout[5][50]  = TILE_KEY;
     */
}
