#ifndef NOVEL_SCENE_H
#define NOVEL_SCENE_H

#include "../../GAME_DATA.h"

void ep1_play_opening(WINDOW*, WINDOW*, Player*, MAP_Structure*);
void ep1_play_ending(WINDOW*, WINDOW*, Player*, MAP_Structure*);  // ← add
void ep1_check_triggers(WINDOW*, WINDOW*, Player*, MAP_Structure*);
void show_cutscene(WINDOW*, Noval_Character, int);
const char *get_message(int);

#endif
