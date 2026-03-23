#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <ncurses.h>

// Tile types
#define TILE_WALL '#'
#define TILE_ITEM1 'C'    // chocolate
#define TILE_ITEM2 'j'    // chocolate
#define TILE_EXIT1 'E' 
#define TILE_EXIT2 'X'
#define TILE_EXIT3 'I'
#define TILE_CAGE '|'

// WINDOW GAME
#define WG_HEIGHT 35
#define WG_WIDTH 90
#define WG_Y 2
#define WG_X 3

// // WINDOW STATUS
// #define WS_HEIGHT 3
// #define WS_WIDTH 60
// #define WS_Y 45
// #define WS_X 3

// WINDOW INVENTORY
#define WI_HEIGHT 3
#define WI_WIDTH 21
#define WI_Y 40
#define WI_X 40

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
  int exit_x, exit_y;
} Map;

// Player structure
typedef struct {
  int health;
  int y, x;
  char inventory[5];
} Player;

// Item structure for pickup
typedef struct {
  char symbol;
  char name[20];
  int value;
  void (*effect)(Player *);
} Item;

typedef struct {
  int game_running;

} GAME_context;

#endif
