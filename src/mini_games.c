#include "GAME_DATA.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>

#include "GAME_DATA.h"
#include "visual_noval.h"

// slot machine --------------------------------------------------------------
static const char list[] = {
  '!', '@', '#', '$',
};

typedef enum {
  SPIN,
  STOP
} STATE;


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

  char c;

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
// ----------------------------------------------------------------------------------------------

// Tic-Tac-Toe
static char board[3][3];
static int player = 1;
static int game_over = 0;
static int winner = 0;
static int cursor_row = 0;
static int cursor_col = 0;
static int attempts = 0;

static void init_board()
{
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

static void draw_board(WINDOW *wind)
{
    werase(wind);
    box(wind, 0, 0);
    
    wattron(wind, A_BOLD);
    mvwprintw(wind, 1, 20, "TIC-TAC-TOE");
    wattroff(wind, A_BOLD);
    
    mvwprintw(wind, 2, 20, "Attempts left: %d", 3 - attempts);

    if (attempts > 2) {
      show_noval_visual(VN_CHAR_U, "you lose");
      game_ctx.game_running = 0;
    }
    
    mvwprintw(wind, 3, 25, " %c | %c | %c ", board[0][0], board[0][1], board[0][2]);
    mvwprintw(wind, 4, 25, "---|---|---");
    mvwprintw(wind, 5, 25, " %c | %c | %c ", board[1][0], board[1][1], board[1][2]);
    mvwprintw(wind, 6, 25, "---|---|---");
    mvwprintw(wind, 7, 25, " %c | %c | %c ", board[2][0], board[2][1], board[2][2]);
    
    mvwprintw(wind, 12, 15, "Arrow keys: Move | SPACE: Place");
    mvwprintw(wind, 13, 18, "q: Quit game");
    
    wrefresh(wind);
}

static void draw_cursor(WINDOW *wind)
{
    wattron(wind, A_REVERSE);
    mvwprintw(wind, 3 + cursor_row * 2, 25 + cursor_col * 4, " %c ", board[cursor_row][cursor_col]);
    wattroff(wind, A_REVERSE);
    wrefresh(wind);
}

static int check_win()
{
    for(int i = 0; i < 3; i++) {
        if(board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return (board[i][0] == 'X') ? 1 : 2;
    }
    for(int j = 0; j < 3; j++) {
        if(board[0][j] != ' ' && board[0][j] == board[1][j] && board[1][j] == board[2][j])
            return (board[0][j] == 'X') ? 1 : 2;
    }
    if(board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return (board[0][0] == 'X') ? 1 : 2;
    if(board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return (board[0][2] == 'X') ? 1 : 2;
    return 0;
}

static int check_tie()
{
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(board[i][j] == ' ') return 0;
    return 1;
}

static int make_move(int row, int col)
{
    if(board[row][col] != ' ') return 0;
    board[row][col] = (player == 1) ? 'X' : 'O';
    return 1;
}

static void ai_move()
{
    int empty[9][2], count = 0;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(board[i][j] == ' ') {
                empty[count][0] = i;
                empty[count][1] = j;
                count++;
            }
    if(count > 0) {
        int r = rand() % count;
        board[empty[r][0]][empty[r][1]] = 'O';
    }
}

void tic_tac_toe_game()
{
    WINDOW *wind = game_ctx.wind[2];
    nodelay(stdscr, FALSE);
    keypad(stdscr, TRUE);
    srand(time(NULL));

not_yet:
    init_board();
    game_over = 0;
    winner = 0;
    player = 1;
    cursor_row = 0;
    cursor_col = 0;

    while(!game_over) {
        draw_board(wind);

        if(player == 1) {
            draw_cursor(wind);
            int ch = getch();

            switch(ch) {
                case KEY_UP:    if(cursor_row > 0) cursor_row--; break;
                case KEY_DOWN:  if(cursor_row < 2) cursor_row++; break;
                case KEY_LEFT:  if(cursor_col > 0) cursor_col--; break;
                case KEY_RIGHT: if(cursor_col < 2) cursor_col++; break;
                case ' ':
                    if(make_move(cursor_row, cursor_col)) {
                        winner = check_win();
                        if(winner)         game_over = 1;
                        else if(check_tie()) { winner = 3; game_over = 1; }
                        else               player = 2;
                    }
                    break;
            }
        } else {
            mvwprintw(wind, 11, 18, "AI thinking...");
            wrefresh(wind);
            napms(500);
            ai_move();
            winner = check_win();
            if(winner)           game_over = 1;
            else if(check_tie()) { winner = 3; game_over = 1; }
            else                 player = 1;
        }
    }

    draw_board(wind);

    if(winner == 1) {
        // WIN — exit
        mvwprintw(wind, 5, 3, "YOU WIN! Press SPACE");
        wrefresh(wind);
        flushinp();
        wrefresh(wind);
        while(getch() != ' ');
        nodelay(stdscr, TRUE);
        return;
    } else {
        // LOSE or TIE — reset
        if(winner == 2)
            mvwprintw(wind, 11, 18, "YOU LOSE! Try again...");
        else
            mvwprintw(wind, 11, 20, "TIE! Try again...");
        attempts++;
        wrefresh(wind);
        napms(1000);  // short pause then auto-reset
        goto not_yet;
    }
}
