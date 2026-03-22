#include "drwing.h"

#define TWO_ROWS 7

static const char *art[4][3][TWO_ROWS] = {

    /* ── 0  DEAD ──────────────────────────────── */
    {{
        " +---------+ ",
        " |  X    X | ",
        " +----+----+ ",
        "      |      ",
        "      .      ",
        "     ...     ",
        "             ",
    },{
        " +---------+ ",
        " |  X    X | ",
        " +----+----+ ",
        "      |      ",
        "      .      ",
        "    . . .    ",
        "             ",
    },{
        " +---------+ ",
        " |  x    x | ",
        " +----+----+ ",
        "      |      ",
        "      .      ",
        "     ...     ",
        "             ",
    }},

    /* ── 1  BAD ───────────────────────────────── */
    {{
        " +---------+ ",
        " |  x    x | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  x    - | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  -    x | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    }},

    /* ── 2  MEH ───────────────────────────────── */
    {{
        " +---------+ ",
        " |  -    - | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  .    - | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  -    . | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    }},

    /* ── 3  HAPPY ─────────────────────────────── */
    {{
        " +---------+ ",
        " |  ^    ^ | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  o    o | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    },{
        " +---------+ ",
        " |  ^    ^ | ",
        " +----+----+ ",
        "      |      ",
        "      |      ",
        "     / \\    ",
        "             ",
    }},
};

/* color pair IDs — must match what utils.c registers */
static inline int color_for(int hp)
{
    if (hp == 3) return 3; /* green  = HAPPY */
    if (hp == 2) return 2; /* yellow = MEH   */
    if (hp == 1) return 1; /* red    = BAD   */
    return 6;              /* white  = DEAD  */
}
 
void draw_two(WINDOW *win, int top, int left, int hp, int frame)
{
    /* clamp inputs so we never go out of bounds */
    if (hp    < 0) hp    = 0;
    if (hp    > 3) hp    = 3;
    if (frame < 0) frame = 0;
    if (frame > 2) frame = 2;
 
    int pair = color_for(hp);
 
    for (int r = 0; r < TWO_ROWS; r++) {
        const char *line = art[hp][frame][r];
        for (int c = 0; line[c]; c++) {
            char ch = line[c];
 
            int bold =
                /* eyes row */
                (r == 1 && (ch=='^'||ch=='o'||ch=='-'||ch=='.'||ch=='x'||ch=='X')) ||
                /* border rows */
                ((r==0||r==2) && (ch=='+'||ch=='-'||ch=='|')) ||
                /* stem */
                ((r==3||r==4) && ch=='|');
 
            if (bold) wattron(win,  COLOR_PAIR(pair) | A_BOLD);
            else      wattron(win,  COLOR_PAIR(pair));
 
            mvwaddch(win, top + r, left + c, ch);
 
            if (bold) wattroff(win, COLOR_PAIR(pair) | A_BOLD);
            else      wattroff(win, COLOR_PAIR(pair));
        }
    }
}
