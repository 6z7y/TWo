#ifndef NOVEL_SCENE_H
#define NOVEL_SCENE_H

#include "../../GAME_DATA.h"

const char *get_message(int msg_id);
static inline const char *get_name(int who);
void show_cutscene(WINDOW *win, Noval_Character who, int msg_id);

#endif
