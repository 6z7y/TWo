#ifndef CONTROL_H
#define CONTROL_H

#include "GAME_DATA.h"


int handle_control(int c, Player *player, MAP_Structure *map);

void player_heal_up(Player *p);
void player_heal_dn(Player *p);

int inventory_checker();
void swap_value_inventory(void);

void use_item(Player *player, int slot);
void pick_up(MAP_Structure *map, Player *player, int y, int x);
void player_heal_dn_swap_control(Player *p);

void move_player(Player *player, MAP_Structure *map, int dy, int dx);

#endif
