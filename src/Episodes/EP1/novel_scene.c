#include <ncurses.h>
#include <unistd.h>   // for usleep

#include "ep.h"
#include "../EP2/render_map.h"
#include "novel_scene.h"
#include "../../drwing.h"  // for draw_color_raw — see below

// ── EP1 story ─────────────────────────────────────
static const Cutscene_Step ep1_opening[] = {
    { 9, 80, CHAR_H,  1 },   // H at pos, says msg 1
    { 6, 20, CHAR_T,  2 },   // T same pos, replies
    { 8, 24, CHAR_T, -1 },   // T walks, no dialogue
    { 8, 24, CHAR_H,  4 },   // H threatens
};

#define EP1_OPENING_LEN 4

static const Cutscene_Step ep1_ending[] = {
    { 32, 3, CHAR_T,  3  },  // T walks to exit, says msg 3
    { 32, 3, CHAR_H,  0  },  // H says "..."
    { 32, 3, CHAR_T, 10  },  // T: "journey begins"
};
#define EP1_ENDING_LEN 3

// each episode owns its own trigger list
static Scene_Trigger ep1_triggers[] = {
    { 6,  30, 0, ep1_play_opening },  // near H
    { 32,  3, 0, ep1_play_ending  },  // at exit
};
#define EP1_TRIGGERS_LEN 2
// ── ASCII art (fixed: y+row offset, mvwprintw not mvprintw) ──

static inline void draw_T_art(WINDOW *w, int y, int x) {
    mvwprintw(w, y+0, x, "  +-----+  ");
    mvwprintw(w, y+1, x, "  | - - |  ");
    mvwprintw(w, y+2, x, "  |  >  |  ");
    mvwprintw(w, y+3, x, "  +--+--+  ");
    mvwprintw(w, y+4, x, "   / | \\  ");
    mvwprintw(w, y+5, x, "  [_] [_]  ");
}

static inline void draw_H_art(WINDOW *w, int y, int x) {
    mvwprintw(w, y+0, x, " /=======\\ ");
    mvwprintw(w, y+1, x, " |# . . #| ");
    mvwprintw(w, y+2, x, " |# ___ #| ");
    mvwprintw(w, y+3, x, " \\==+==/ " );
    mvwprintw(w, y+4, x, "   /| |\\  ");
    mvwprintw(w, y+5, x, "  [=] [=]  ");
}

static inline void draw_W_art(WINDOW *w, int y, int x) {
    mvwprintw(w, y+0, x, " +-------+ ");
    mvwprintw(w, y+1, x, " | o   o | ");
    mvwprintw(w, y+2, x, " |  \\_/  | ");
    mvwprintw(w, y+3, x, " +---+---+ ");
    mvwprintw(w, y+4, x, "  /  |  \\ ");
    mvwprintw(w, y+5, x, " /   |   \\");
}

static inline void draw_F_art(WINDOW *w, int y, int x) {
    mvwprintw(w, y+0, x, "  +-----+  ");
    mvwprintw(w, y+1, x, "  | . - |  ");
    mvwprintw(w, y+2, x, "  |  ~  |  ");
    mvwprintw(w, y+3, x, "  +--+--+  ");
    mvwprintw(w, y+4, x, "   / |     ");
    mvwprintw(w, y+5, x, "  [_]      ");
}

// ── messages ──────────────────────────────────────
const char *get_message(int msg_id) {
    switch (msg_id) {
        case 0:  return "...";
        case 1:  return "You dare challenge me?!";
        case 2:  return "I have come to stop you!";
        case 3:  return "This is not over yet!";
        case 4:  return "You will regret this!";
        case 10: return "Welcome. Your journey begins.";
        case 11: return "The kingdom needs you.";
        default: return "[unknown]";
    }
}

static inline const char *get_name(Noval_Character who) {
    switch (who) {
        case CHAR_T: return "T";
        case CHAR_W: return "W";
        case CHAR_H: return "H";
        case CHAR_F: return "F";
        default:     return "?";
    }
}

// ── show one dialogue line in wind_noval ──────────
void show_cutscene(WINDOW *win, Noval_Character who, int msg_id)
{
    werase(win);
    box(win, 0, 0);

    // Zone 1: portrait — fixed y+row offset, draws into win
    switch (who) {
        case CHAR_T: draw_T_art(win, 2, 8); break;
        case CHAR_H: draw_H_art(win, 2, 8); break;
        case CHAR_W: draw_W_art(win, 2, 8); break;
        case CHAR_F: draw_F_art(win, 2, 8); break;
    }

    // Zone 2: speaker name — get_name not get_message!
    wattron(win, A_BOLD);
    mvwprintw(win, 10, 2, "[ %s ]", get_name(who));
    wattroff(win, A_BOLD);

    // Zone 3: separator + message
    mvwhline(win, 11, 1, ACS_HLINE, WS_WIDTH - 2);
    mvwprintw(win, 13, 2, "%s", get_message(msg_id));

    // Zone 4: hint
    mvwprintw(win, WS_HEIGHT - 2, 2, "[ SPACE = next ]");

    wrefresh(win);

    // block until SPACE — user reads, then continues
    nodelay(stdscr, FALSE);          // blocking mode
    while (wgetch(win) != ' ');
    nodelay(stdscr, TRUE);           // back to non-blocking
}

// ── draw map helper (wind_game only) ──────────────
static void draw_map_frame(WINDOW *wind_game,
                            MAP_Structure *map,
                            Player *player)
{
    werase(wind_game);
    box(wind_game, 0, 0);

    // draw tiles
    for (int i = 0; i < WG_HEIGHT; i++) {
        for (int j = 0; map->layout[i][j]; j++) {
            char t = map->layout[i][j];
            if (t == TILE_CAGE) wattron(wind_game, A_DIM);
            mvwaddch(wind_game, i + 1, j + 1, t);
            if (t == TILE_CAGE) wattroff(wind_game, A_DIM);
        }
    }

    // draw player T
    wattron(wind_game, A_BOLD);
    mvwprintw(wind_game, player->y + 1, player->x + 1, "T");
    wattroff(wind_game, A_BOLD);

    // draw H enemy — fixed position for EP1 opening
    wattron(wind_game, A_BOLD);
    mvwprintw(wind_game, 6 + 1, 35 + 1, "H");
    wattroff(wind_game, A_BOLD);

    wnoutrefresh(wind_game);
    doupdate();
}

// ── the core engine ───────────────────────────────
void play_cutscene(
    WINDOW *wind_game,
    WINDOW *wind_noval,
    Player *player,
    MAP_Structure *map,
    const Cutscene_Step *steps,
    int n)
{
    for (int i = 0; i < n; i++) {
        const Cutscene_Step *s = &steps[i];

        // move player if step says so
        if (s->player_x >= 0) {
            player->x = s->player_x;
            player->y = s->player_y;
        }

        // small pause so movement feels cinematic
        usleep(300000);  // 0.3 sec — tweak this

        // redraw map with new player position
        draw_map_frame(wind_game, map, player);

        // show dialogue if this step has one
        if (s->msg_id >= 0)
            show_cutscene(wind_noval, s->who, s->msg_id);
    }

    // clear novel window when story is done
    werase(wind_noval);
    box(wind_noval, 0, 0);
    mvwprintw(wind_noval, 0, 2, " Story ");
    wnoutrefresh(wind_noval);
    doupdate();
}

// ── EP1 entry point ───────────────────────────────
void ep1_play_opening(WINDOW *wind_game, WINDOW *wind_noval,
                      Player *player, MAP_Structure *map)
{
    play_cutscene(wind_game, wind_noval, player, map,
                  ep1_opening, EP1_OPENING_LEN);
}

// ── EP1 ending entry point ────────────────────────
void ep1_play_ending(WINDOW *wind_game, WINDOW *wind_noval,
                     Player *player, MAP_Structure *map)
{
    play_cutscene(wind_game, wind_noval, player, map,
                  ep1_ending, EP1_ENDING_LEN);

    // ── transition to EP2 ──────────────────────
    game_ctx.ep = EP2_PRISON_BREAK;
    load_episode2(&game_ctx.map);

    // reset player position to EP2 start
    player->x = game_ctx.map.player_start_x;
    player->y = game_ctx.map.player_start_y;

    // back to game — now EP2 map is loaded
    game_ctx.state = STATE_GAME;
}

void ep1_check_triggers(
    WINDOW *wind_game,
    WINDOW *wind_noval,
    Player *player,
    MAP_Structure *map)
{
    for (int i = 0; i < EP1_TRIGGERS_LEN; i++) {
        Scene_Trigger *t = &ep1_triggers[i];

        if (t->fired) continue;             // already played, skip

        if (player->y == t->trigger_y &&
            player->x == t->trigger_x) {
            t->fired = 1;                   // mark BEFORE calling
            t->fn(wind_game, wind_noval, player, map);
        }
    }
}
