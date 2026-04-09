#include <string.h>
#include <ncurses.h>

#include "render_map.h"
#include "../../utils.h"

/* ── episode 1 : Cave of Secrets ────────────────── */
void load_episode3(MAP_Structure *map)
{
    /* basic info */
    strncpy(map->name, "Cave of Secrets", sizeof(map->name));
    map->player_start_x = 0;
    map->player_start_y = 0;
    map->exit_x         = 40;
    map->exit_y         = 15;

    /* layout — copy each row */
    strncpy(map->layout[ 0], "                                                                                      ", WG_WIDTH);
    strncpy(map->layout[ 1], "                                                                                      ", WG_WIDTH);
    strncpy(map->layout[ 2], "                      x                                                                ", WG_WIDTH);
    strncpy(map->layout[ 3], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[ 4], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[ 5], " S                                                                                     ", WG_WIDTH);
    strncpy(map->layout[ 6], "                                 x                                                     ", WG_WIDTH);
    strncpy(map->layout[ 7], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[ 8], "                   x                                     x                             ", WG_WIDTH);
    strncpy(map->layout[ 9], "                                        x                                              ", WG_WIDTH);
    strncpy(map->layout[10], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[11], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[12], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[13], "                     x          x              x                                       ", WG_WIDTH);
    strncpy(map->layout[14], "                                                                          x            ", WG_WIDTH);
    strncpy(map->layout[15], "              x                                                                        ", WG_WIDTH);
    strncpy(map->layout[16], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[17], "                                                            x                          ", WG_WIDTH);
    strncpy(map->layout[18], "                               x           x                                           ", WG_WIDTH);
    strncpy(map->layout[19], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[20], "                                                                          x            ", WG_WIDTH);
    strncpy(map->layout[21], "                                                                       x               ", WG_WIDTH);
    strncpy(map->layout[22], "                     x                                                                 ", WG_WIDTH);
    strncpy(map->layout[23], "              x                                                                        ", WG_WIDTH);
    strncpy(map->layout[24], "                                     x                 x                               ", WG_WIDTH);
    strncpy(map->layout[25], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[26], "                               x                                        x              ", WG_WIDTH);
    strncpy(map->layout[27], "                                                                                       ", WG_WIDTH);
    strncpy(map->layout[28], "||||||||||   x                                                                         ", WG_WIDTH);
    strncpy(map->layout[29], "         |                  x                            x                        x    ", WG_WIDTH);
    strncpy(map->layout[30], "         |                                                                             ", WG_WIDTH);
    strncpy(map->layout[31], "         |                                    x                        x          x    ", WG_WIDTH);
    strncpy(map->layout[32], "[EXI ]   |                                                                             ", WG_WIDTH);
}
