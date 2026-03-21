/*
 * two.c — TWo, the T character
 * Compile: gcc two.c -o two -lncurses
 * Run:     ./two
 */

#include <ncurses.h>
#include <stdlib.h>

#define ROWS   7
#define FRAMES 3

#define C_HAPPY 1
#define C_MEH   2
#define C_BAD   3
#define C_DEAD  4

static const char *art[4][FRAMES][ROWS] = {
    /* 0 DEAD */
    {{" +---------+ ",
      " |  X     X  |",
      " +----+----+ ",
      "      |      ",
      "      .      ",
      "     ...     ",
      "             "},
     {" +---------+ ",
      " |  X     X  |",
      " +----+----+ ",
      "      |      ",
      "      .      ",
      "    . . .    ",
      "             "},
     {" +---------+ ",
      " |  x     x  |",
      " +----+----+ ",
      "      |      ",
      "      .      ",
      "     ...     ",
      "             "}},
    /* 1 BAD */
    {{" +---------+ ",
      " |  x     x  |",
      " +----+----+ ",
      "      |      ",
      "      |      ",
      "     / \\    ",
      "             "},
     {" +---------+ ",
      " |  x     -  |",
      " +----+----+ ",
      "      |      ",
      "      |      ",
      "     / \\    ",
      "             "},
     {" +---------+ ",
      " |  -     x  |",
      " +----+----+ ",
      "      |      ",
      "      |      ",
      "     / \\    ",
      "             "}},
    /* 2 MEH */
    {{" +---------+ ",
      " |  -     -  |",
      " +----+----+ ",
      "      |      ",
      "      |      ",
      "     / \\    ",
      "             "},
     {" +---------+ ",
      " |  .     -  |",
      " +----+----+ ",
      "      |      ",
      "      |      ",
      "     / \\    ",
      "             "},
     {" +---------+ ",
      " |  -     .  |",
      " +----+----+ ",
      "      |      ",
      "      |      ",
      "     / \\    ",
      "             "}},
    /* 3 HAPPY */
    {{" +---------+ ",
      " |  ^     ^  |",
      " +----+----+ ",
      "      |      ",
      "      |      ",
      "     / \\    ",
      "             "},
     {" +---------+ ",
      " |  o     o  |",
      " +----+----+ ",
      "      |      ",
      "      |      ",
      "     / \\    ",
      "             "},
     {" +---------+ ",
      " |  ^     ^  |",
      " +----+----+ ",
      "      |      ",
      "      |      ",
      "     / \\    ",
      "             "}},
};

static int color_for(int hp)
{
    if (hp == 3) return C_HAPPY;
    if (hp == 2) return C_MEH;
    if (hp == 1) return C_BAD;
    return C_DEAD;
}

static void draw_art(int top, int left, int hp, int frame)
{
    int pair = color_for(hp);
    for (int r = 0; r < ROWS; r++) {
        const char *line = art[hp][frame][r];
        for (int c = 0; line[c]; c++) {
            char ch = line[c];
            int bold = 0;
            if (r == 1 && (ch=='^'||ch=='o'||ch=='-'||ch=='.'||ch=='x'||ch=='X')) bold=1;
            if ((r==0||r==2) && (ch=='+'||ch=='-'||ch=='|')) bold=1;
            if ((r==3||r==4) && ch=='|') bold=1;
            if (bold) attron(COLOR_PAIR(pair)|A_BOLD);
            else      attron(COLOR_PAIR(pair));
            mvaddch(top+r, left+c, ch);
            if (bold) attroff(COLOR_PAIR(pair)|A_BOLD);
            else      attroff(COLOR_PAIR(pair));
        }
    }
}

int main(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    // nodelay(stdscr, TRUE);
    timeout(600);

    start_color();
    use_default_colors();
    init_pair(C_HAPPY, COLOR_GREEN,  -1);
    init_pair(C_MEH,   COLOR_YELLOW, -1);
    init_pair(C_BAD,   COLOR_RED,    -1);
    init_pair(C_DEAD,  COLOR_WHITE,  -1);

    int hp=3, frame=0, tick=0, ch;

    while (1) {
        ch = getch();
        if (ch=='q'||ch=='Q') break;
        if ((ch==KEY_UP  ||ch=='k') && hp<3) hp++;
        if ((ch==KEY_DOWN||ch=='j') && hp>0) hp--;

        int my, mx;
        getmaxyx(stdscr, my, mx);
        (void)my;

        erase();
        draw_art((my-ROWS)/2, (mx-15)/2, hp, frame);
        refresh();

        frame = (frame+1) % FRAMES;
        tick++;
    }

    endwin();
    return 0;
}
