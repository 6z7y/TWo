#include <string.h>
#include <ncurses.h>
#include <stdlib.h>

#include "control.h"
#include "GAME_DATA.h"


int handle_control(int c, Player *player, Map *map)
{
  switch(c) {
//            KEY                            do
    case KEY_UP:    case 'k': move_player(player, map, 0, -1);  break;
    case KEY_DOWN:  case 'j': move_player(player, map, 0, 1);   break;
    case KEY_LEFT:  case 'h': move_player(player, map, -1, 0);  break;
    case KEY_RIGHT: case 'l': move_player(player, map, 1, 0);   break;


    case '+': if (player->health <= 3) player_heal_up(player); break;
    case '-': if (player->health > 0) player_heal_dn(player); break;

    case 'Q':                                                   return 1;
  }
  
  return 0; 
}

char *health_ascii(int health)
{
    if (health == 3) return "❤ ❤ ❤";
    if (health == 2) return "❤ ❤  ";
    if (health == 1) return "❤    ";
    return "x x x";
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

// Check if tile is walkable
int is_walkable(Map *map, int x, int y, Player *player) {
    if(x < 0 || y < 0 || y >= WG_HEIGHT || x >= (int)strlen(map->layout[y])) {
        return 0;
    }
    
    char tile = map->layout[y][x];
    
    // Walkable tiles
    if(tile == TILE_FLOOR || tile == ' ') return 1;
    
    // Locked door - need key
    // if(tile == TILE_DOOR && player->has_key) return 1;
    // if(tile == TILE_DOOR && !player->has_key) return 0;
    
    // Items are walkable (will be picked up)
    if(tile == TILE_KEY || tile == TILE_TREASURE || 
       tile == TILE_POTION || tile == TILE_SWORD || tile == '?') return 1;
    
    // Exit is walkable
    if(tile == TILE_EXIT) return 1;
    
    return 0;
}

// Move player
void move_player(Player *player, Map *map, int dx, int dy)
{
    int new_x = player->x + dx;
    int new_y = player->y + dy;
    
    if(is_walkable(map, new_x, new_y, player)) {
        char tile = map->layout[new_y][new_x];
        
        // Check for items to pick up
        if(tile == TILE_KEY || tile == TILE_TREASURE || 
           tile == TILE_POTION || tile == TILE_SWORD || tile == '?') {
            // pickup_item(map, player, new_x, new_y);
        }
        
        // Check for exit
        if(tile == TILE_EXIT) {
            mvprintw(LINES - 3, 4, "Congratulations! You escaped the cave!");
            refresh();
            napms(2000);
            exit(0);
        }
        
        // Move player
        player->x = new_x;
        player->y = new_y;
    }
}
