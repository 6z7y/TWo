#include "GAME_DATA.h"
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

static const char list[] = {
  '!', '@', '#', '$',
};

typedef enum {
  SPIN,
  STOP
} STATE;

char c;

void slot_machine_game()
{
  WINDOW *wind = game_ctx.wind[2];
  werase(wind);
  box(wind, 0, 0);
not_yet:
  STATE s1 = SPIN;
  STATE s2 = SPIN;
  STATE s3 = SPIN;
  int i1 = 0;
  int i2 = 0;
  int i3 = 0;

  mvwprintw(wind, 5, 30, "GOAL: '$' '$' '$'");

  while(!(s1 == STOP && s2 == STOP && s3 == STOP)) {

    // col 1
    mvwprintw(wind, 2, 4, "%c", list[(i1 + 2) % 4]);
    mvwprintw(wind, 3, 4, "%c", list[(i1 + 1) % 4]);
    mvwprintw(wind, 4, 4, "%c", list[i1 % 4]);

    // col 2
    mvwprintw(wind, 2, 6, "%c", list[(i2 + 2) % 4]);
    mvwprintw(wind, 3, 6, "%c", list[(i2 + 1) % 4]);
    mvwprintw(wind, 4, 6, "%c", list[i2 % 4]);

    // col 3
    mvwprintw(wind, 2, 8, "%c", list[(i3 + 2) % 4]);
    mvwprintw(wind, 3, 8, "%c", list[(i3 + 1) % 4]);
    mvwprintw(wind, 4, 8, "%c", list[i3 % 4]);

    // input
    c = getch();
    if (c == ' ') {
      if (s1 == SPIN) s1 = STOP;
      else if (s2 == SPIN) s2 = STOP;
      else if (s3 == SPIN) s3 = STOP;
    }

    if (s1 == SPIN) i1++;
    if (s2 == SPIN) i2++;
    if (s3 == SPIN) i3++;
    wrefresh(wind);
    usleep(400000);
  }

  mvwprintw(wind, 6, 4, "Result: %c %c %c", list[(i1 + 1) % 4], list[(i2 + 1) % 4], list[(i3 + 1) % 4]);
  if (list[(i1 + 1) % 4] == '$' && list[(i2 + 1) % 4] == '$' && list[(i3 + 1) % 4] == '$') {
    mvwprintw(wind, 7, 4, "u win! Press [Space]");
    wrefresh(wind);

  } else {
    mvwprintw(wind, 7, 4, "u lose reset!!");
    goto not_yet;

  }

  // Wait for space to continue
  while (getch() != ' ');
  wrefresh(wind);
}
