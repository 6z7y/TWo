#ifndef DRAWING_H
#define DRAWING_H

#include "GAME_DATA.h"
#include <ncurses.h>

void draw_two(WINDOW *win, int top, int left, int hp, int frame);
void update_T_animation(int *anim_tick, int *frame);
void render_game(WINDOW *wind_game, WINDOW *wind_inventory, Define_Episode ep, Player *player, MAP_Structure *map);

#endif
