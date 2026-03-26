#include <ncurses.h>
#include <string.h>
#include "GAME_DATA.h"

#include "Episodes/include_helper.h"
#include "control.h"
#include "drwing.h"

#define TWO_ROWS 7

static const char *art[4][3][TWO_ROWS] = {

    /* ── 0  DEAD ──────────────────────────────── */
    {{
        " +---------+ ",
        " |  X    X | ",
        " +----+----+ ",
        "      |      ",
        "      .      ",
        "     ...     ",
        "             ",
    },{
        " +---------+ ",
        " |  X    X | ",
        " +----+----+ ",
        "      |      ",
        "      .      ",
        "    . . .    ",
        "             ",
    },{
        " +---------+ ",
        " |  x    x | ",
        " +----+----+ ",
        "      |      ",
        "      .      ",
        "     ...     ",
        "             ",
    }},

    /* ── 1  BAD ───────────────────────────────── */
    {{
        " +---------+ ",
        " |  x    x | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  x    - | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  -    x | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    }},

    /* ── 2  MEH ───────────────────────────────── */
    {{
        " +---------+ ",
        " |  -    - | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  .    - | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  -    . | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    }},

    /* ── 3  HAPPY ─────────────────────────────── */
    {{
        " +---------+ ",
        " |  ^    ^ | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  o    o | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  ^    ^ | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    }},
};

char *health_ascii(int health)
{
    if (health == 3) return "❤ ❤ ❤";
    if (health == 2) return "❤ ❤  ";
    if (health == 1) return "❤    ";
    return "x x x";
}

void draw_map(WINDOW *win, MAP_Structure *map, Player *player) {
    for (int i = 0; i < WG_HEIGHT; i++) { // row
        for (int j = 0; j < (int)strlen(map->layout[i]); j++) { // col
            char tile = map->layout[i][j];

            if      (tile == TILE_WALL)                                              wattron(win, COLOR_PAIR(1));
            else if (tile == TILE_ITEM1 || tile == TILE_ITEM2)                       wattron(win, COLOR_PAIR(3)); // green like items
            else if (tile == TILE_EXIT1 || tile == TILE_EXIT2 || tile == TILE_EXIT3) wattron(win, COLOR_PAIR(5));
            else if (tile == TILE_CAGE)                                              wattron(win, COLOR_PAIR(1));

            mvwaddch(win, i + 1, j + 1, tile); /////////////////////////////////////////////////////////////////////////// here i stop

            wattroff(win, COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4) | COLOR_PAIR(5));
        }
    }
    wattron(win, A_BOLD | COLOR_PAIR(1));
    mvwprintw(win, player->y + 1, player->x + 1, "T");
    wattroff(win, A_BOLD | COLOR_PAIR(1));
}

void render_game(WINDOW *wind_game, WINDOW *wind_inventory, Episode ep, Player *player, int frame, MAP_Structure *map)
{
  werase(stdscr); // clear screen

  // title
  mvprintw(0, 4, " TWo ");

  // title wind game
  wattron(wind_game, A_UNDERLINE);
  mvwprintw(wind_game, 0, 2, " Ep1: kanojo helper ");
  wattroff(wind_game, A_UNDERLINE);

  // helper
  mvprintw(LINES - 2, 4, "Controls: (Arrow/Vim keys): Move | (+/-): incress/decress health | (e): inventory open | (Q) = Quit");

  // border 
  box(stdscr, 0, 0);
  box(wind_game, 0, 0);
  box(wind_inventory, 0, 0);

  draw_map(wind_game, map, player);
  (void)ep;


    mvwprintw(stdscr, 45, 3, "Health: %s", health_ascii(player->health) );

    werase(wind_inventory);

    if (is_inventory_open()) {
        /* big inventory window */
        wresize(wind_inventory, 12, 40);
        mvwin(wind_inventory,   20, 20);
        box(wind_inventory, 0, 0);
        mvwprintw(wind_inventory, 0, 2, " INVENTORY — press 1-5 to use, e to close ");

        for (int i = 0; i < 5; i++) {
            char item = player->inventory[i];
            const char *name = (item == TILE_ITEM1) ? "Chocolate" :
                               (item == TILE_ITEM2) ? "Candy"     : "-- empty --";
            if (item == '.') {
                mvwprintw(wind_inventory, 2 + i * 2, 3,
                    "[%d]  .  %s", i + 1, name);
            } else {
                wattron(wind_inventory, COLOR_PAIR(3) | A_BOLD);
                mvwprintw(wind_inventory, 2 + i * 2, 3,
                    "[%d]  %c  %s", i + 1, item, name);
                wattroff(wind_inventory, COLOR_PAIR(3) | A_BOLD);
            }
        }

        mvwprintw(wind_inventory, 11, 3, " press e to close ");

    } else {
        /* small bar — always visible */
        wresize(wind_inventory, WIC_HEIGHT, WIC_WIDTH);
        mvwin(wind_inventory,   WIC_Y, WIC_X);
        box(wind_inventory, 0, 0);
        mvwprintw(wind_inventory, 0, 2, " bag ");
        mvwprintw(wind_inventory, 1, 1, " %c | %c | %c | %c | %c ",
            player->inventory[0], player->inventory[1],
            player->inventory[2], player->inventory[3],
            player->inventory[4]);
        mvwprintw(wind_inventory, 2, 2, " press e ");
    }

    draw_two(stdscr, 38, 4, player->health, frame);
}

/* color pair IDs — must match what utils.c registers */
static inline int color_for(int hp)
{
    if (hp == 3) return 3; /* green  = HAPPY */
    if (hp == 2) return 2; /* yellow = MEH   */
    if (hp == 1) return 1; /* red    = BAD   */
    return 6;              /* white  = DEAD  */
}

void update_T_animation(int *anim_tick, int *frame)
{
  /* CHANGE 5: simple anim_tick — no double increment bug */
  (*anim_tick)++;
  if (*anim_tick >= 24) {       /* 6 x 16ms = ~100ms per frame */
    *frame = (*frame + 1) % 3;
    *anim_tick = 0;
  }
}
 
void draw_two(WINDOW *win, int top, int left, int hp, int frame)
{
    /* clamp inputs so we never go out of bounds */
    if (hp    < 0) hp    = 0;
    if (hp    > 3) hp    = 3;
    if (frame < 0) frame = 0;
    if (frame > 2) frame = 2;
 
    int pair = color_for(hp);
 
    for (int r = 0; r < TWO_ROWS; r++) {
        const char *line = art[hp][frame][r];
        for (int c = 0; line[c]; c++) {
            char ch = line[c];
 
            int bold =
                /* eyes row */
                (r == 1 && (ch=='^'||ch=='o'||ch=='-'||ch=='.'||ch=='x'||ch=='X')) ||
                /* border rows */
                ((r==0||r==2) && (ch=='+'||ch=='-'||ch=='|')) ||
                /* stem */
                ((r==3||r==4) && ch=='|');
 
            if (bold) wattron(win,  COLOR_PAIR(pair) | A_BOLD);
            else      wattron(win,  COLOR_PAIR(pair));
 
            mvwaddch(win, top + r, left + c, ch);
 
            if (bold) wattroff(win, COLOR_PAIR(pair) | A_BOLD);
            else      wattroff(win, COLOR_PAIR(pair));
        }
    }
}
