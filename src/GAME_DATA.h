#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <ncurses.h>

// Tile types
#define TILE_WALL '#'
#define TILE_CAGE '|'
#define TILE_BOX  '@'   // pushable object
#define TILE_HOLE '_'   // target hole
#define TILE_FILLED '.' // ← reuse '.' or pick new char — hole filled

#define TILE_ITEM1 '%'

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

// WINDOW Second
#define WS_HEIGHT 40
#define WS_WIDTH  70
#define WS_Y      2
#define WS_X      95

// #define NOVAL_CHAR_Y 15
// #define NOVAL_CHAR_X 140

typedef enum {
  CHAR_T,   // Player
  CHAR_W,   // kanojo T
  CHAR_P,   // Friend = Pixel
  CHAR_F,   // friend = feno
  CHAR_H,   // Boss EP1 = Havoc
  CHAR_D,   // Boss EP2 = Dread
  CHAR_S,   // Boss EP3 = Shade
  CHAR_U,   // Boss EP4 / Final = Ultim

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
// ----------------

typedef struct {
    int        player_y;   // where to move player on wind_game
    int        player_x;   // -1 = don't move, keep current pos
    Noval_Character who;   // who speaks on wind_noval
    int        msg_id;     // -1 = no dialogue this step, just move
} Cutscene_Step;

typedef struct {
  int trigger_y;          // tile position that fires this
  int trigger_x;
  int fired;              // 0 = not yet, 1 = already played
  void (*fn)(WINDOW*, WINDOW*, Player*, MAP_Structure*);
} Scene_Trigger;

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
  WINDOW *wind[NUM_WINDOW];
  Define_Episode ep; // define current episode
  MAP_Structure map; // episode map
  Game_State state; // state playing
  Player player;    // player structure

} GAME_Context;
// -----------------

extern GAME_Context game_ctx;
#endif
