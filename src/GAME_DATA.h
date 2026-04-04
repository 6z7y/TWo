#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <ncurses.h>

#define file_db "TWo_save.db"

// Tile types
#define TILE_CAGE '|'
#define TILE_ROCK  '@'   // pushable object
#define TILE_HOLE '_'   // target hole
// #define TILE_FILLED '.' // ← reuse '.' or pick new char — hole filled

#define TILE_GRASS_1 ','
#define TILE_GRASS_2 '\''
#define TILE_GRASS_3 '"'

#define TILE_ITEM1 '%'
#define TILE_ITEM3 '^'
#define TILE_ITEM2 '*'

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
#define CHAR_P 'P' // Friend = Pixel
#define CHAR_F 'F' // friend = feno
#define CHAR_H 'H' // Boss EP1 = Havoc
#define CHAR_D 'D' // Boss EP2 = Dread
#define CHAR_S 'S' // Boss EP3 = Shade
#define CHAR_U 'U' // Boss EP4 / Final = Ultim

typedef enum {
  NV_CHAR_T,   // Player
  NV_CHAR_W,   // kanojo T
  NV_CHAR_P,   // Friend = Pixel
  NV_CHAR_F,   // friend = feno
  NV_CHAR_H,   // Boss EP1 = Havoc
  NV_CHAR_D,   // Boss EP2 = Dread
  NV_CHAR_S,   // Boss EP3 = Shade
  NV_CHAR_U,   // Boss EP4 / Final = Ultim

} Noval_Character;

// for episode
typedef enum {
    EP1_KIDNAPPING = 1,
    EP2_PRISON_BREAK = 2,
    EP3_BOSS_FIGHT = 3,
    EP4_HAPPY_ENDING = 4
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

typedef struct {
    int        player_y;   // where to move player on wind_game
    int        player_x;   // -1 = don't move, keep current pos
    Noval_Character who;   // who speaks on wind_noval
    int        msg_id;     // -1 = no dialogue this step, just move
} Cutscene_Step;

typedef struct {
  int fired;              // 0 = not yet, 1 = already played
  int trigger_y;          // tile position that fires this
  int trigger_x;
  void (*fn)(WINDOW*, WINDOW*, Player*, MAP_Structure*);
} Scene_Trigger;


// structure context
typedef struct {
  int game_running;
  WINDOW *wind[NUM_WINDOW];
  Define_Episode ep; // define current episode
  MAP_Structure map; // episode map
  Player player;    // player structure

} GAME_Context;

extern GAME_Context game_ctx;

#endif
