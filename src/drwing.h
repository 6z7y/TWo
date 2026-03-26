#ifndef DRAWING_H
#define DRAWING_H

#include "GAME_DATA.h"
#include <ncurses.h>

int select_episode();
void setup_user_for_ep(Player *player, Episode *ep, MAP_Structure *episode1_struct);
void render_game(WINDOW *wind_game, WINDOW *wind_inventory, Episode ep, Player *player, int frame, MAP_Structure *episode1_struct);
void draw_two(WINDOW *win, int top, int left, int hp, int frame);
void update_T_animation(int *anim_tick, int *frame);

#endif
