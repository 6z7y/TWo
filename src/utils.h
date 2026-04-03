#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>

#include "GAME_DATA.h"
#include "Episodes/include_helper.h"

// static inline void setup_user_for_map(Player *player, Define_Episode *ep, MAP_Structure *episode1_struct);
int select_episode();
void setup_game(GAME_Context *game_ctx);
void ncurses_mode(int mode);
void warn_term_size(int y, int x);

#endif
