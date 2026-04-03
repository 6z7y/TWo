#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "control.h"
#include "GAME_DATA.h"
#include "noval_visual.h"

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
  switch(c) {
    case KEY_UP:    case 'k': move_player(player, map, -1, 0);  break;
    case KEY_DOWN:  case 'j': move_player(player, map, 1, 0);   break;
    case KEY_LEFT:  case 'h': move_player(player, map, 0, -1);  break;
    case KEY_RIGHT: case 'l': move_player(player, map, 0, 1);   break;
    case 'e':                 swap_value_inventory();           break;
    case '+': if (player->health < 3) player_heal_up(player);   break;
    case '-': if (player->health > 0) player_heal_dn(player);   break;
    case 'Q':                                                   return 1;
  }
  return 0;
}

/* use item from slot i — define effects here */
void use_item(Player *player, int slot)
{
  if (player->inventory[slot] == '.') return; /* empty slot */
 
  char item = player->inventory[slot];
 
  if (item == TILE_ITEM1) {
    player_heal_up(player);               /* chocolate heals */
  }
 
  player->inventory[slot] = ' ';          /* consume item */
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
}

void player_heal_dn(Player *p)
{
  p->health--;
  if (p->health < 1) p->health = 0;
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

static int ep1_scene1_fired = 0;

// Check if tile is walkable
int is_walkable(MAP_Structure *map, int x, int y) {
    if(x < 0 || y < 0 || y >= WG_HEIGHT || x >= (int)strlen(map->layout[y])) {
        return 0;
    }
    
    char tile = map->layout[y][x];
    
    // Walkable tiles
    if (tile == ' ')      return 1;
    if (tile == TILE_ITEM1) return 1;
    if (tile == TILE_GRASS_1 || tile == TILE_GRASS_2 || tile == TILE_GRASS_3) return 1;

    // visual noval
    if (game_ctx.ep == EP1_KIDNAPPING) {
      if (game_ctx.player.y == 5 && game_ctx.player.x == 2 && !ep1_scene1_fired) {
        ep1_scene1_fired = 1;
        show_noval_visual(NV_CHAR_H, 3);
        show_noval_visual(NV_CHAR_U, 3);
      }
    }
    
    return 0;
}
// Move player
void move_player(Player *player, MAP_Structure *map, int dy, int dx)
{
    int new_x = player->x + dx;
    int new_y = player->y + dy;

    if (new_y < 0 || new_y >= WG_HEIGHT) return;
    if (new_x < 0 || new_x >= WG_WIDTH)  return;

    char tile = map->layout[new_y][new_x];


    // ── rock push ──────────────────────────────────
    if (tile == TILE_ROCK) {
        if (!try_push(map, new_y, new_x, dy, dx))
            return;  // push blocked — player stays
        // push succeeded — player moves into box's old spot
        player->x = new_x;
        player->y = new_y;
        return;
    }

    if (player->y == 32 && player->x == 4) {
      // game_ctx.game_running = 0;  // or set a win state
      // ep1_play_ending(game_ctx.wind[0], game_ctx.wind[1], player, &game_ctx.map);

    }

    // ── normal walkable ───────────────────────────
    if (is_walkable(map, new_x, new_y)) {
        if (tile == TILE_ITEM1) pick_up(map, player, new_y, new_x);

        player->x = new_x;
        player->y = new_y;

        char t = map->layout[new_y][new_x];
        // if (t == TILE_EXIT1 || t == TILE_EXIT2 || t == TILE_EXIT3)
        //     game_ctx.state = STATE_CUTSCENE;
    }
}
