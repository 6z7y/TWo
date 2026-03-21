#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <ncurses.h>

// Tile types
#define TILE_WALL '#'
#define TILE_FLOOR '.'
#define TILE_DOOR '+'
#define TILE_KEY 'K'
#define TILE_TREASURE '$'
#define TILE_POTION '!'
#define TILE_SWORD '/'
#define TILE_EXIT 'E'

// WINDOW GAME
#define WG_HEIGHT 35
#define WG_WIDTH 90
#define WG_Y 2
#define WG_X 3

// WINDOW STATUS
#define WS_HEIGHT 3
#define WS_WIDTH 60
#define WS_Y 38
#define WS_X 5

// WINDOW INVENTORY
#define WI_HEIGHT 3
#define WI_WIDTH 18
#define WI_Y 41
#define WI_X 5

// for episode
typedef enum {
    EP1_KIDNAPPING = 1,
    EP2_PRISON_BREAK = 2,
    EP3_BOSS_FIGHT = 3,
    EP4_HAPPY_ENDING = 4
} Episode;

typedef struct {
  WINDOW *wind;
} NEW_Wind;


typedef struct {
  char name[32];
  char layout[WG_HEIGHT][WG_WIDTH + 1];
  int player_start_x, player_start_y;
  int key_x, key_y;
} Map;

// Player structure
typedef struct {
  int health;
  int y, x;
} Player;

// Item structure for pickup
typedef struct {
  char symbol;
  char name[20];
  int value;
  void (*effect)(Player *);
} Item;

#endif
