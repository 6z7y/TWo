#ifndef SAVE_H
#define SAVE_H

#include <sqlite3.h>

#include "GAME_DATA.h"
#include "Episodes/include_helper.h"

void save_episode(int num);
void load_current_episode(Define_Episode *ep);

#endif
