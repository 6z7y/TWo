#include <ncurses.h>
#include <string.h>
#include "GAME_DATA.h"

#include "Episodes/include_helper.h"
#include "control.h"
#include "drwing.h"

#define TWO_ROWS 7

void draw_color(WINDOW *win, MAP_Structure *map, Player *player);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <pthread.h>
#include <unistd.h>


// ── shared state ──────────────────────────────────
typedef struct {
    WINDOW *win;
    int     active;   // 1 = player at 1hp, 0 = stop
} CRT_Effect;

static CRT_Effect crt = { NULL, 0 };
static pthread_t  crt_thread;
static pthread_mutex_t crt_mutex = PTHREAD_MUTEX_INITIALIZER;

// ── the thread ────────────────────────────────────
void *crt_worker(void *arg) {
    (void)arg;
    int t = 0;

    while (1) {
        pthread_mutex_lock(&crt_mutex);
        if (!crt.active) {
            pthread_mutex_unlock(&crt_mutex);
            break;
        }
        WINDOW *win = crt.win;
        int gy, gx;
        getmaxyx(win, gy, gx);

        int scan = t % (gy - 5);
        wattron(win, A_REVERSE);
        mvwhline(win, scan + 1, 1, ' ', gx - 2);
        wattroff(win, A_REVERSE);

        int tear = (t * 17) % (gy - 2) + 1;
        wattron(win, A_DIM);
        mvwhline(win, tear, 2, '-', gx - 4);
        wattroff(win, A_DIM);

        for (int i = 0; i < 30; i++) {
            int r = (t * 7  + i * 13) % (gy - 2) + 1;
            int c = (t * 11 + i * 17) % (gx - 2) + 1;
            mvwaddch(win, r, c, ("|/-%#~"[i % 6]) | A_DIM);
        }

        // ── NO wrefresh() here ──
        touchwin(win);   // just mark dirty

        pthread_mutex_unlock(&crt_mutex);
        t++;
        usleep(60000);
    }
    return NULL;
}

// ── call these from your game ─────────────────────

void crt_start(WINDOW *win) {
    pthread_mutex_lock(&crt_mutex);
    crt.win    = win;
    crt.active = 1;
    pthread_mutex_unlock(&crt_mutex);
    pthread_create(&crt_thread, NULL, crt_worker, NULL);
}

void crt_stop(void) {
    pthread_mutex_lock(&crt_mutex);
    crt.active = 0;
    pthread_mutex_unlock(&crt_mutex);
    pthread_join(crt_thread, NULL);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

const char *health_ascii(int health)
{
    if (health == 3) return "❤ ❤ ❤";
    if (health == 2) return "❤ ❤  ";
    if (health == 1) return "❤    ";
    return "x x x";
}

/* color pair IDs — must match what utils.c registers */
static inline int color_for_T(int hp)
{
    if (hp == 3) return 4; /* green  = HAPPY */
    if (hp == 2) return 9; /* yellow = MEH   */
    if (hp == 1) return 2; /* red    = BAD   */
    return 8;              /* white  = DEAD  */
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
 
    int pair = color_for_T(hp);
 
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

static int frame     = 0; // for animation T
static int anim_tick = 0;

void render_game(WINDOW *wind_game, WINDOW *wind_inventory, Define_Episode ep, Player *player, MAP_Structure *map)
{
    // ── 1. erase ALL windows first ──────────────────
    werase(stdscr);

    // ── 2. borders ──────────────────────────────────
    box(stdscr,        0, 0);
    box(wind_game,     0, 0);
    // box(wind_noval,    0, 0);

    // ── 3. titles (AFTER erase, AFTER box) ──────────
    mvprintw(0, 4, " TWo ");                          // stdscr title — works now

    wattron(wind_game, A_UNDERLINE);
    mvwprintw(wind_game, 0, 2, " %d: %s ", game_ctx.ep, game_ctx.map.name);
    wattroff(wind_game, A_UNDERLINE);

    // wattron(wind_noval, A_UNDERLINE);
    // mvwprintw(wind_noval, 0, 2, " Story ");
    // wattroff(wind_noval, A_UNDERLINE);

    // ── 4. controls hint ────────────────────────────
    mvprintw(LINES - 2, 4, "Controls: [Arrow/(hjkl)]: Move | (+/-): health | (e): bag | (Q) Quit");

    // ── 5. game map ─────────────────────────────────
    draw_color(wind_game, map, player);

    // ── 6. health bar ───────────────────────────────
    mvwprintw(stdscr, 45, 3, "Health: %s", health_ascii(player->health));
    mvwprintw(stdscr, 47, 3, "x: %d, y: %d", game_ctx.player.x, game_ctx.player.y);

    // ── 7. inventory ────────────────────────────────
    werase(wind_inventory);
    if (inventory_checker()) {
        wresize(wind_inventory, WIO_HEIGHT, WIO_WIDTH);
        mvwin(wind_inventory,   WIO_Y, WIO_X);
        box(wind_inventory, 0, 0);
        mvwprintw(wind_inventory, 0, 2, " INVENTORY -- press 1-5 to use ");
        for (int i = 0; i < 5; i++) {
            char item = player->inventory[i];
            const char *name = (item == TILE_ITEM1) ? "pieces pizza" : "-- empty --";
            if (item == '.') {
                mvwprintw(wind_inventory, 2 + i * 2, 3, "[%d]  .  %s", i + 1, name);
            } else {
                // wattron(wind_inventory, COLOR_PAIR(3) | A_BOLD);
                mvwprintw(wind_inventory, 2 + i * 2, 3, "[%d]  %c  %s", i + 1, item, name);
                // wattroff(wind_inventory, COLOR_PAIR(3) | A_BOLD);
            }
        }
        mvwprintw(wind_inventory, 11, 3, " press e to close ");
    } else {
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

    // ── 8. T character animation ─────────────────────
    update_T_animation(&anim_tick, &frame);
    draw_two(stdscr, 38, 4, player->health, frame);

    // ── 9. CRT effect ────────────────────────────────
    if (player->health == 1 && !crt.active) {
        crt_start(wind_game);
    } else if (player->health > 1 && crt.active) {
        crt_stop();
        pthread_mutex_lock(&crt_mutex);
        werase(wind_game);
        box(wind_game, 0, 0);
        wattron(wind_game, A_UNDERLINE);
        mvwprintw(wind_game, 0, 2, " Ep1: kanojo helper ");
        wattroff(wind_game, A_UNDERLINE);
        pthread_mutex_unlock(&crt_mutex);
    }
}
