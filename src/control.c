#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

#include "control.h"
#include "GAME_DATA.h"

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
 
  if (item == TILE_ITEM1 || item == TILE_ITEM2) {
    player_heal_up(player);               /* chocolate heals */
  }
 
  player->inventory[slot] = '.';          /* consume item */
}

int inventory_checker()
{
  return inventory_mode;
}

static inline void swap_value_inventory(void)
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

void pick_up(MAP_Structure *map, Player *player, int y, int x)
{
  char item = map->layout[y][x]; // get position

  for (int i=0; i<5; i++) { // check solt inventory
    if (player->inventory[i] == '.') { // empty solt
      player->inventory[i] = item;
      map->layout[y][x] = ' ';
      return;
    }
  }
  // inventory full!
}

// Check if tile is walkable
int is_walkable(MAP_Structure *map, int x, int y, Player *player) {
    if(x < 0 || y < 0 || y >= WG_HEIGHT || x >= (int)strlen(map->layout[y])) {
        return 0;
    }
    
    char tile = map->layout[y][x];
    
    // Walkable tiles
    
    if(tile == ' ' || tile == TILE_ITEM1 || tile == TILE_ITEM2) return 1;
    
    return 0;
}

// Move player
void move_player(Player *player, MAP_Structure *map, int dy, int dx)
{
    int new_x = player->x + dx;
    int new_y = player->y + dy;
    
    if(is_walkable(map, new_x, new_y, player)) {
        char tile = map->layout[new_y][new_x];
        
        if (map->layout[new_y][new_x] == TILE_ITEM1)  pick_up(map, player, new_y, new_x);
        if (map->layout[new_y][new_x] == TILE_ITEM2)  pick_up(map, player, new_y, new_x);
        
        // Check for exit
        // if(tile == TILE_EXIT) {
        //     mvprintw(LINES - 3, 4, "Congratulations! You escaped the cave!");
        //     refresh();
        //     napms(2000);
        //     exit(0);
        // }
        
        // Move player
        player->x = new_x;
        player->y = new_y;
    }
}
