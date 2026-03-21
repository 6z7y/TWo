#ifndef CONTROL_H
#define CONTROL_H

#include "GAME_DATA.h"

int handle_control(int c, Player *player, Map *map);
void move_player(Player *player, Map *map, int dx, int dy);

#endif
