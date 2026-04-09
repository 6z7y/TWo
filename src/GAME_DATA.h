#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <ncurses.h>
#include <time.h>

#define file_db "TWo_save.db"

// functions
#define LEN(x) (sizeof(x)/sizeof(x[0]))
#define for_each_arr(a) for (int i=0; i<LEN(a); i++)
#define for_each_num(a) for (int i=0; i<a; i++)

// Tile types
#define TILE_CAGE '|'
#define TILE_ROCK  '@'   // pushable object
#define TILE_HOLE '_'   // target hole
// #define TILE_FILLED '.' // ← reuse '.' or pick new char — hole filled

#define TILE_GRASS_1 ','
#define TILE_GRASS_2 '\''
#define TILE_GRASS_3 '"'

#define ITEM1 '%' // pizza
#define ITEM2 '*' // spoiled cake
#define ITEM3 '^' // Bulldozer

#define TILE_EXIT1 'E' 
#define TILE_EXIT2 'X'
#define TILE_EXIT3 'I'

// how much window
#define NUM_WINDOW 4

// 1 WINDOW GAME
#define WG_HEIGHT 35
#define WG_WIDTH 90 // wid = 90 + \0
#define WG_Y 2
#define WG_X 3

// 2 WINDOW SECOND
#define WS_HEIGHT 35
#define WS_WIDTH 25
#define WS_Y 2
#define WS_X 94

// 3 WINDOW Noval_visual
#define WN_HEIGHT 10
#define WN_WIDTH  70
#define WN_Y      24
#define WN_X      15

// 4 WINDOW INVENTORY (Close/Open)
#define WIC_HEIGHT 3
#define WIC_WIDTH 21
#define WIC_Y 10
#define WIC_X 96

#define WIO_HEIGHT 14
#define WIO_WIDTH 26
#define WIO_Y 10
#define WIO_X 95

// #define NOVAL_CHAR_Y 15
// #define NOVAL_CHAR_X 140

#define CHAR_T 'T' // Player
#define CHAR_W 'W' // kanojo T
#define CHAR_F 'F' // friend = feno
#define CHAR_H 'P' // Boss EP1 = Pixel
#define CHAR_D 'D' // Boss EP2 = Dread
#define CHAR_S 'S' // Boss EP3 = Shade
#define CHAR_U 'U' // Boss EP4 / Final = Ultim

typedef enum {
  NV_CHAR_T,   // Player
  NV_CHAR_W,   // kanojo T
  NV_CHAR_F,   // friend = feno
  NV_CHAR_P,   // Boss EP1 = Pixel
  NV_CHAR_D,   // Boss EP2 = Dread
  NV_CHAR_S,   // Boss EP3 = Shade
  NV_CHAR_U,   // Boss EP4 / Final = Ultim

} Noval_Character;

// for episode
typedef enum {
    EP1 = 1,
    EP2 = 2,
    EP3 = 3,
    EP4 = 4
} Define_Episode;
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
// -------------==

// Item structure for pickup
typedef struct {
  char symbol;
  char name[20];
  int value;
  void (*effect)(Player *);
} Item;
// ----------------

// structure context
typedef struct {
  WINDOW *wind[NUM_WINDOW];
  Define_Episode ep; // define current episode
  MAP_Structure map; // episode map
  Player player;    // player structure
  int game_running;
  int reload_game;
  int timer;
  int swap_control;
  int rocks;
  time_t start, now;

} GAME_Context;

extern GAME_Context game_ctx;

#endif
