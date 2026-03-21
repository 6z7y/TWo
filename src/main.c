#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "GAME_DATA.h"
#include "control.h"
#include "episodes.h"
#include "menu.h"
#include "utils.h"

// Item effects
Map episode1_struct = {
    .name = "Cave of Secrets",
    .player_start_x = 15,
    .player_start_y = 6,
    // .exit_x = 70,
    // .exit_y = 30,
    // .has_key = 0,
    .key_x = 40,
    .key_y = 15,
    // .treasure_x = 50,
    // .treasure_y = 20,
    .layout = {
        "########################################################################################",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#         ||||||||||||||||||                                                           #",
        "#         |                |                                                           #",
        "#         |                |                                                           #",
        "#         |                |                                                           #",
        "#         |                |                                                           #",
        "#         ||||||||||||||||||                                                           #",
        "#             E                                                                        #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "#                                                                                      #",
        "########################################################################################"
    }
};

// // Item definitions
// Item items[] = {
//     {'!', "Health Potion", 20, heal_player},
//     {'$', "Treasure", 100, add_score},
//     {'K', "Golden Key", 0, get_key},
//     {'/', "Iron Sword", 0, get_sword},
//     {'?', "Mystery Box", 50, add_score}
// };



// // Pick up item at position
// void pickup_item(Map *map, Player *player, int x, int y) {
//     char tile = map->layout[y][x];
//
//     for(int i = 0; i < sizeof(items)/sizeof(Item); i++) {
//         if(items[i].symbol == tile) {
//             // Apply effect
//             if(items[i].effect) {
//                 items[i].effect(player);
//             }
//
//             // Add to inventory
//             strcpy(player->inventory[player->item_count], items[i].name);
//             player->item_count++;
//
//             // Show message
//             char msg[100];
//             sprintf(msg, "Picked up: %s!", items[i].name);
//             mvprintw(LINES - 3, 4, "%s", msg);
//
//             // Remove item from map
//             map->layout[y][x] = TILE_FLOOR;
//
//             // Special case for key
//             if(tile == TILE_KEY) {
//                 player->has_key = 1;
//                 mvprintw(LINES - 3, 4, "You got the KEY! Now you can open doors!");
//             }
//             break;
//         }
//     }
// }

void draw_map(WINDOW *win, Map *map, Player *player) {
    for(int i = 0; i < WG_HEIGHT - 1 && i < WG_HEIGHT; i++) {
        if(strlen(map->layout[i]) > 0) {
            // Apply color based on tile type
            for(int j = 0; j < WG_WIDTH - 2 && j < (int)strlen(map->layout[i]); j++) {
                char tile = map->layout[i][j];
                int color = 0;
                
                if(tile == TILE_WALL) {
                    wattron(win, COLOR_PAIR(1)); // Walls - red
                } else if(tile == TILE_DOOR) {
                    wattron(win, COLOR_PAIR(2)); // Doors - yellow
                } else if(tile == TILE_KEY || tile == TILE_TREASURE) {
                    wattron(win, COLOR_PAIR(3)); // Items - green
                } else if(tile == TILE_POTION) {
                    wattron(win, COLOR_PAIR(4)); // Potion - magenta
                } else if(tile == TILE_EXIT) {
                    wattron(win, COLOR_PAIR(5)); // Exit - cyan
                }
                
                mvwaddch(win, i + 1, j + 1, tile);
                
                if(tile != TILE_FLOOR && tile != ' ') {
                    wattron(win, COLOR_PAIR(0));
                }
            }
        }
    }
    
    // Draw player on top
    wattron(win, A_BOLD | COLOR_PAIR(6));
    mvwprintw(win, player->y + 1, player->x + 1, "T");
    wattron(win, COLOR_PAIR(0));
}

void render_game(NEW_Wind *wind_game, NEW_Wind *wind_status, NEW_Wind *wind_inventory, int episode, Player *player) {
  // Clear windows
  // clear();
  // werase(wind_game->wind);
  // werase(wind_status->wind);
  // werase(wind_inventory->wind);
  
  // Draw borders
  box(stdscr, 0, 0);
  box(wind_game->wind, 0, 0);
  box(wind_status->wind, 0, 0);
  box(wind_inventory->wind, 0, 0);
  
  // Main screen info
  mvprintw(0, 4, " TWo ");
  mvprintw(LINES - 2, 4, "Controls: Arrow/Vim keys = Move | Q = Quit");
  
  // Game window
  if(episode == 1) {
    draw_map(wind_game->wind, &episode1_struct, player);
  }
  
  // Status window
  mvwprintw(wind_status->wind, 1, 2, "Health: %d/3", player->health);
  
  // Inventory window
  mvwprintw(wind_inventory->wind, 1, 2, "  |  |  |  |  ");
  // for(int i = 0; i < player->item_count && i < 4; i++) {
    // mvwprintw(wind_inventory->wind, 1, 10 + (i * 12), "%s", player->inventory[i]);
  // }
}

int main(void) {

  int c; // key input
  int y, x; // position terminal

  // 1. init tui (ncurses)
  ncurses_mode(1);

  // 2. Select Episode
  Episode ep = EP1_KIDNAPPING;
  int current_episode = ep;

  // 3. init window
  NEW_Wind w_game =      { init_window_now(WG_HEIGHT, WG_WIDTH, WG_Y, WG_X) }; // Game window
  NEW_Wind w_status =    { init_window_now(WS_HEIGHT, WS_WIDTH, WS_Y, WS_X) }; // Status window
  NEW_Wind w_inventory = { init_window_now(WI_HEIGHT, WI_WIDTH, WI_Y, WI_X) }; // Inventory window

  // 4. Initialize player
  Player player = {0};
  if (ep == EP1_KIDNAPPING) player = (Player){.health=1, .y=episode1_struct.player_start_y, .x=episode1_struct.player_start_x}; // ep1
  // ..

  // 5. main loop
  while(1) {
    getmaxyx(stdscr, y, x); // get position terminal
    
    if (x < 40 || y < 40) { 
      warn_term_size(&y, &x); // warn user
      continue; 
    }

    // Movement
    c = getch();
    if (handle_control(c, &player, &episode1_struct) == 1) break;
    
    render_game(&w_game, &w_status, &w_inventory, current_episode, &player); // drawing
    
    // refresh
    // NEW — queue everything, then ONE single write to terminal
    wnoutrefresh(stdscr);
    wnoutrefresh(w_game.wind);
    wnoutrefresh(w_status.wind);
    wnoutrefresh(w_inventory.wind);
    doupdate();  // ← only this touches the terminal
    
    usleep(16000);       // ~60fps cap
    
  }

  ncurses_mode(0);
  return 0;
}
