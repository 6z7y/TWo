#include <string.h>
#include "GAME_DATA.h"

#include "Episodes/include_helper.h"
#include "drwing.h"

#define TWO_ROWS 7

/* map is filled by load_episode1() in main */

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

void draw_map(WINDOW *win, Map *map, Player *player) {
    for (int i = 0; i < WG_HEIGHT - 1; i++) {
        if (strlen(map->layout[i]) > 0) {
            for (int j = 0; j < WG_WIDTH - 2 && j < (int)strlen(map->layout[i]); j++) {
                char tile = map->layout[i][j];

                // if      (tile == TILE_WALL)                         wattron(win, COLOR_PAIR(1));
                // else if (tile == TILE_ITEM)                         wattron(win, COLOR_PAIR(3)); // green like items
                // else if (tile == TILE_EXIT)                         wattron(win, COLOR_PAIR(5));

                mvwaddch(win, i + 1, j + 1, tile);

                // wattroff(win, COLOR_PAIR(1) | COLOR_PAIR(2) | COLOR_PAIR(3) | COLOR_PAIR(4) | COLOR_PAIR(5));
            }
        }
    }
    // wattron(win, A_BOLD | COLOR_PAIR(6));
    mvwprintw(win, player->y + 1, player->x + 1, "T");
    // wattroff(win, A_BOLD | COLOR_PAIR(6));
}


/* CHANGE 2: int frame not int* */
void render_game(NEW_Wind *wind_game, NEW_Wind *wind_inventory, int episode, Player *player, int frame, Map *episode1_struct)
{
    werase(stdscr);

    box(stdscr, 0, 0);
    box(wind_game->wind, 0, 0);
    // box(wind_status->wind, 0, 0);
    box(wind_inventory->wind, 0, 0);

    mvprintw(0, 4, " TWo ");

    wattron(wind_game->wind, A_UNDERLINE);
    mvwprintw(wind_game->wind, 0, 2, " Ep1: kanojo helper ");
    wattroff(wind_game->wind, A_UNDERLINE);

    mvprintw(LINES - 2, 4, "Controls: (Arrow/Vim keys): Move | (+/-): incress/decress health | (Q) = Quit");

    if (episode == 1) {
      
        draw_map(wind_game->wind, episode1_struct, player);
    }

    mvwprintw(stdscr, 45, 3, "Health: %s", health_ascii(player->health) );

    mvwprintw(wind_inventory->wind, 1, 1, " %c | %c | %c | %c | %c ",
        player->inventory[0], player->inventory[1], player->inventory[2], player->inventory[3], player->inventory[4]);

    draw_two(stdscr, 38, 4, player->health, frame);
}

int select_episode()
{
  Episode ep = EP1_KIDNAPPING;
  return ep;

}

void setup_user_for_ep(Player *player, int *current_episode, Map *episode1_struct)
{
  if (*current_episode == EP1_KIDNAPPING)
    *player = (Player){ .health = 1, .y = episode1_struct->player_start_y, .x = episode1_struct->player_start_x };
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
 
            // if (bold) wattron(win,  COLOR_PAIR(pair) | A_BOLD);
            // else      wattron(win,  COLOR_PAIR(pair));
 
            mvwaddch(win, top + r, left + c, ch);
 
            // if (bold) wattroff(win, COLOR_PAIR(pair) | A_BOLD);
            // else      wattroff(win, COLOR_PAIR(pair));
        }
    }
}
