#include <ncurses.h>
#include <string.h>
#include "GAME_DATA.h"

#include "Episodes/include_helper.h"
#include "control.h"
#include "drwing.h"

void setup_color()
{
  start_color();

  if (can_change_color()) {
    init_color(COLOR_BLACK,      0,    0,    0);
    init_color(COLOR_RED,     1000,    0,    0);
    init_color(COLOR_BLUE,       0,    0, 1000);
    init_color(COLOR_GREEN,      0, 1000,    0);
    init_color(COLOR_YELLOW,  1000, 1000,    0);
    init_color(COLOR_MAGENTA, 1000,    0, 1000);
    init_color(COLOR_CYAN,       0, 1000, 1000);
    init_color(COLOR_WHITE,   1000, 1000, 1000);
    init_color(9, 1000, 2550, 0); // Orange
    init_color(10, 500, 250, 0); // brown
  }

  // assume_default_colors(COLOR_WHITE, COLOR_BLACK);

  init_pair(1, 10,     10);   // 1: tile + bg red
  init_pair(2, COLOR_RED,     COLOR_BLACK); // 2: health 1
  init_pair(3, COLOR_BLUE,    COLOR_BLACK); // 3: ???
  init_pair(4, COLOR_GREEN,   COLOR_BLACK); // 4: grass & health 3
  init_pair(5, COLOR_YELLOW,  COLOR_BLACK); // 5: Player [T]
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // 6: characters
  init_pair(7, COLOR_CYAN,    COLOR_BLACK); // 7: Exit
  init_pair(8, COLOR_WHITE,   COLOR_BLACK); // 8: other
  init_pair(9, 9, COLOR_BLACK); // 9
  init_pair(10, 10, COLOR_BLACK); // 10 items
}

void draw_color(WINDOW *win, MAP_Structure *map, Player *player)
{
    for (int i = 0; i < WG_HEIGHT; i++) { // row
        for (int j = 0; j < (int)strlen(map->layout[i]); j++) { // col
            char tile = map->layout[i][j];
            
            // Apply color based on tile type
            if (tile == TILE_CAGE) {
              wattron(win, COLOR_PAIR(1));  // Red for cages
            }

            else if (tile == TILE_ITEM1 || tile == TILE_ITEM2 || tile == TILE_ITEM3) {
                wattron(win, COLOR_PAIR(9));  // Brown for items (you had 3, but 4 is GREEN)
            }

            else if (tile == TILE_EXIT1 || tile == TILE_EXIT2 || tile == TILE_EXIT3) {
                wattron(win, COLOR_PAIR(7));  // Cyan for exits (you had 5)
            }

            else if (tile == CHAR_H || tile == CHAR_D || tile == CHAR_S || tile == CHAR_U) {
                wattron(win, COLOR_PAIR(6) | A_BOLD);  // Magenta with bold for special objects
            }

            else if (tile == TILE_ROCK) {  // Player character
                wattron(win, COLOR_PAIR(10));  // Brown for player
            }

            else if (tile == TILE_GRASS_1 || tile == TILE_GRASS_2 || tile == TILE_GRASS_3) {
              wattron(win, COLOR_PAIR(4));
            }
            
            // Draw the character
            mvwaddch(win, i + 1, j + 1, tile);
            
            // Turn off ONLY the color that was turned on
            // Better approach: save the attribute before turning on, then restore
            if (tile == TILE_CAGE) {
              wattroff(win, COLOR_PAIR(1));
            }

            else if (tile == TILE_ITEM1 || tile == TILE_ITEM2 || tile == TILE_ITEM3) {
                wattroff(win, COLOR_PAIR(9));
            }

            else if (tile == TILE_EXIT1 || tile == TILE_EXIT2 || tile == TILE_EXIT3) {
                wattroff(win, COLOR_PAIR(7));
            }

            else if (tile == CHAR_H || tile == CHAR_D || tile == CHAR_S || tile == CHAR_U) {
                wattroff(win, COLOR_PAIR(6) | A_BOLD);
            }

            else if (tile == TILE_ROCK) {
                wattroff(win, COLOR_PAIR(10));
            }

            else if (tile == TILE_GRASS_1 || tile == TILE_GRASS_2 || tile == TILE_GRASS_3) {
              wattroff(win, COLOR_PAIR(4));
            }
        }
    }
    
    // Draw player marker with color 6
    wattron(win, A_BOLD | COLOR_PAIR(6));  // Changed from 5 to 6
    mvwprintw(win, player->y + 1, player->x + 1, "T");
    wattroff(win, A_BOLD | COLOR_PAIR(6));
}
