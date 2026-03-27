#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <ncurses.h>

// Tile types
#define TILE_WALL '#'
#define TILE_CAGE '|'

#define TILE_ITEM1 'C'
#define TILE_ITEM2 'K'

#define TILE_EXIT1 'E' 
#define TILE_EXIT2 'X'
#define TILE_EXIT3 'I'

// how much window
#define NUM_WINDOW 3

// WINDOW GAME
#define WG_HEIGHT 35
#define WG_WIDTH 90 // wid = 90 + \0
#define WG_Y 2
#define WG_X 3

// WINDOW INVENTORY (Close/Open)
#define WIC_HEIGHT 3
#define WIC_WIDTH 21
#define WIC_Y 40
#define WIC_X 40

#define WIO_HEIGHT 14
#define WIO_WIDTH 37
#define WIO_Y 38
#define WIO_X 36

// WINDOW Novel (right side)
#define WN_HEIGHT 40
#define WN_WIDTH  70
#define WN_Y      2
#define WN_X      95

// #define NOVAL_CHAR_Y 15
// #define NOVAL_CHAR_X 140

typedef enum {
  CHAR_T,   // Player
  CHAR_W,   // kanojo T
  CHAR_H,   // Boss EP1
  CHAR_F    // friend

} Noval_Character;

typedef enum {
  STATE_GAME,
  STATE_CUTSCENE,
  STATE_MENU,
} Game_State;

// for episode
typedef enum {
    EP1_KIDNAPPING = 1,
    EP2_PRISON_BREAK = 2,
    EP3_BOSS_FIGHT = 3,
    EP4_HAPPY_ENDING = 4
} Episode;
// -----------

// about ep map
typedef struct {
  char name[32];
  char layout[WG_HEIGHT][WG_WIDTH];
  int player_start_x, player_start_y;
  int exit_x, exit_y;
} MAP_Structure;
// ------------

// Player structure
typedef struct {
  int health;
  int y, x;
  char inventory[5];
} Player;
// ----------------

// Item structure for pickup
typedef struct {
  char symbol;
  char name[20];
  int value;
  void (*effect)(Player *);
} Item;
// -------------------------

// structure context
typedef struct {
  int game_running;
  Game_State state;
  WINDOW *wind[NUM_WINDOW];
  MAP_Structure map;
  Episode ep;
  Player player;

} GAME_Context;
// -----------------

extern GAME_Context game_ctx;
#endif
