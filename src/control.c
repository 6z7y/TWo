#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "control.h"
#include "GAME_DATA.h"
#include "visual_noval.h"
#include "save.h"


static int inventory_mode = 0;

int handle_control(int c, Player *player, MAP_Structure *map)
{
  /* when inventory is open, only 1-5 and e work */
  if (inventory_mode) {
    switch(c) {
      case '1': use_item(player, 0);    break;
      case '2': use_item(player, 1);    break;
      case '3': use_item(player, 2);    break;
      case '4': use_item(player, 3);    break;
      case '5': use_item(player, 4);    break;
      case 'e': swap_value_inventory(); break;
      case 'Q':                         return 1;
    }
    return 0;
  }
 
  /* normal game controls */
  if (game_ctx.swap_control) {
    switch(c) {
      case KEY_DOWN:    case 'j': move_player(player, map, -1, 0);  break;
      case KEY_UP:  case 'k': move_player(player, map, 1, 0);   break;
      case KEY_RIGHT:  case 'l': move_player(player, map, 0, -1);  break;
      case KEY_LEFT: case 'h': move_player(player, map, 0, 1);   break;
      case 'e':                 swap_value_inventory();           break;
      case 'Q':                                                   return 1;
    } 
  }
  else {
    switch(c) {
      case KEY_UP:    case 'k': move_player(player, map, -1, 0);  break;
      case KEY_DOWN:  case 'j': move_player(player, map, 1, 0);   break;
      case KEY_LEFT:  case 'h': move_player(player, map, 0, -1);  break;
      case KEY_RIGHT: case 'l': move_player(player, map, 0, 1);   break;
      case 'e':                 swap_value_inventory();           break;
      case 'Q':                                                   return 1;
    }
  }
  return 0;
}


/* use item from slot i — define effects here */
void use_item(Player *player, int slot)
{
  if (player->inventory[slot] == ' ') return; /* empty slot */
 
  char item = player->inventory[slot];
 
  if (item == ITEM1) {
    player_heal_up(player);
    player->inventory[slot] = ' ';          /* consume item */
  }
  else if (item == ITEM2) {
    player_heal_dn_swap_control(player);
    player->inventory[slot] = ' ';          /* consume item */
  }

  if (item == ITEM3) {
    return;
  }
 
}

int inventory_checker()
{
  return inventory_mode;
}

void swap_value_inventory(void)
{
  inventory_mode = !inventory_mode;
}

void player_heal_up(Player *p)
{ 
  p->health++;
  if (p->health > 3) p->health = 3;

  game_ctx.swap_control = 0;
  game_ctx.timer = 0;
}

void player_heal_dn_swap_control(Player *p)
{
  p->health--;
  if (p->health < 1) p->health = 0;

  game_ctx.swap_control = 1;
  show_noval_visual(VN_CHAR_G, "..hh");
  show_noval_visual(VN_CHAR_T, "what is this");
  inventory_mode = 0;
}

// returns 1 if push succeeded, 0 if blocked
static int try_push(MAP_Structure *map, int box_y, int box_x, int dy, int dx)
{
    int dest_y = box_y + dy;
    int dest_x = box_x + dx;

    // bounds check
    if (dest_y < 0 || dest_y >= WG_HEIGHT) return 0;
    if (dest_x < 0 || dest_x >= WG_WIDTH)  return 0;

    char dest = map->layout[dest_y][dest_x];

    if (dest == TILE_HOLE) {
        // box falls into hole — both disappear, floor is walkable
        map->layout[dest_y][dest_x] = ' ';   // hole filled = walkable
        map->layout[box_y][box_x]   = ' ';   // box gone
        game_ctx.rocks++;
        return 1;
    }

    if (dest == ' ') {
        // box slides to empty floor
        map->layout[dest_y][dest_x] = TILE_ROCK;
        map->layout[box_y][box_x]   = ' ';
        return 1;
    }

    return 0;  // wall or anything else — blocked
}

void dig_with_bulldozer(MAP_Structure *map, Player *player, int y, int x)
{
    // Check if player has bulldozer in inventory
    int has_bulldozer = 0;
    int bulldozer_slot = -1;
    
    for (int i = 0; i < 5; i++) {
        if (player->inventory[i] == ITEM3) {
            has_bulldozer = 1;
            bulldozer_slot = i;
            break;
        }
    }
    
    if (!has_bulldozer) {
        return;
    }
    
    // Dig on 'x' tile
    if (map->layout[y][x] == 'x') {
        map->layout[y][x] = ' ';  // Remove the dig spot
        
        // ONLY give key at specific position (example: position 15, 15)
        // Change these coordinates to where you want the key to be
        if (y == 21 && x == 71) {
          show_noval_visual(VN_CHAR_T, "i found it the keys '&'");
            // Add key to inventory
            for (int i = 0; i < 5; i++) {
                if (player->inventory[i] == ' ') {
                    player->inventory[i] = ITEM4;
                    return;
                }
            }
        }
    }
}

void pick_up(MAP_Structure *map, Player *player, int y, int x)
{
  char item = map->layout[y][x]; // get position

  for (int i=0; i<5; i++) { // check solt inventory
    if (player->inventory[i] == ' ') { // empty solt
      player->inventory[i] = item;
      map->layout[y][x] = ' ';
      return;
    }
  }
  // inventory full!
}


// Check if tile is walkable
int is_walkable(MAP_Structure *map, int x, int y) {
    if(x < 0 || y < 0 || y >= WG_HEIGHT || x >= (int)strlen(map->layout[y])) {
        return 0;
    }
    
    char tile = map->layout[y][x];
    
    // Walkable tiles
    if (tile == ' ')      return 1;
    if (tile == ITEM1 || tile == ITEM2 || tile == ITEM3) return 1;
    if (tile == TILE_GRASS_1 || tile == TILE_GRASS_2 || tile == TILE_GRASS_3) return 1;

    
    return 0;
}

// Move player
void move_player(Player *player, MAP_Structure *map, int dy, int dx)
{
    int new_x = player->x + dx;
    int new_y = player->y + dy;
    // visual noval
    visual_noval_checker();

    if (new_y < 0 || new_y >= WG_HEIGHT) return;
    if (new_x < 0 || new_x >= WG_WIDTH)  return;

    char tile = map->layout[new_y][new_x];


    // ── rock push ──────────────────────────────────
    if (tile == TILE_ROCK) {
        if (!try_push(map, new_y, new_x, dy, dx)) return;  // push blocked — player stays
        // push succeeded — player moves into box's old spot
        player->x = new_x;
        player->y = new_y;
        return;
    }

    // ── Bulldozer digging on 'x' ──────────────────────────────────
    if (tile == 'x') {
        // Check if player has bulldozer
        int has_bulldozer = 0;
        for (int i = 0; i < 5; i++) {
            if (player->inventory[i] == ITEM3) {
                has_bulldozer = 1;
                break;
            }
        }
        
        if (has_bulldozer) {
            dig_with_bulldozer(map, player, new_y, new_x);
            return;  // Don't move onto the 'x'
        } else {
            show_noval_visual(VN_CHAR_T, "i Need bulldozer (^)");
            return;
        }
    }

    // ── Check exit with key requirement for EP3 ──────────────────
    if (player->y == 32 && player->x == 4) {
        // For EP3, require key to exit
        if (game_ctx.ep == EP3) {
            int has_key = 0;
            int key_slot = -1;
            for (int i = 0; i < 5; i++) {
                if (player->inventory[i] == ITEM4) {
                    has_key = 1;
                    key_slot = i;
                    break;
                }
            }
        }
    }

    if (player->y == 32 && player->x == 4) {
      if (game_ctx.ep == EP1) {
        save_episode(2);
        game_ctx.reload_game = 1;
      }
      else if (game_ctx.ep == EP2) {
        save_episode(3);
        game_ctx.reload_game = 1;
      }
      else if (game_ctx.ep == EP3) {
        save_episode(4);
        game_ctx.reload_game = 1;
      }
      else if (game_ctx.ep == EP4) {
        show_noval_visual(999, "After that...");
        show_noval_visual(999, "Together, [T] and [W] started a new life");
        show_noval_visual(999, "And soon, their family grew");
        show_noval_visual(999, "After marriage, they had a child named [o]");
        show_noval_visual(999, "This is the end of TWo.");
        show_noval_visual(999, "Thanks for playing!");
        show_noval_visual(999, "The Developer: 6z7y");
        game_ctx.game_running = 0;
      }
    }

    // ── normal walkable ───────────────────────────
    if (is_walkable(map, new_x, new_y)) {

        player->x = new_x;
        player->y = new_y;
        if (tile == ITEM1 || tile == ITEM2 || tile == ITEM3) pick_up(map, player, new_y, new_x);

        char t = map->layout[new_y][new_x];
    }
}
