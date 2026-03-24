#ifndef CONTROL_H
#define CONTROL_H

#include "GAME_DATA.h"

int handle_control(int c, Player *player, Map *map);

char *health_ascii(int health);
void player_heal_up(Player *p);
void player_heal_dn(Player *p);

void use_item(Player *player, int slot);
void pick_up(Map *map, Player *player, int y, int x);
void open_inventory(void);
int  is_inventory_open(void);

void move_player(Player *player, Map *map, int dx, int dy);

#endif
