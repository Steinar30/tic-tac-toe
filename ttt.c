#include "ttt.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

char* string_opts[] = {
    " ", "x", "o"
};

int main(int argc, char** argv)
{
    initscr();
    start_color();
    noecho();
    curs_set(0);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);

    int c;
    int x = 0, y = 0;

    board_t* board = init_board();

    keypad(stdscr, TRUE);
    print_board(board, x, y);

    while ((c = getch()) != KEY_F(1)) {
        switch (c) {
        case KEY_DOWN:
            y++;
            move_cursor(board, &x, &y);
            break;

        case KEY_UP:
            y--;
            move_cursor(board, &x, &y);
            break;

        case KEY_RIGHT:
            x++;
            move_cursor(board, &x, &y);
            break;

        case KEY_LEFT:
            x--;
            move_cursor(board, &x, &y);
            break;

        case 10:
            enter_input(board, x, y);
            break;

        case KEY_F(2):
            restart_game(board);
            print_board(board, x, y);
            break;
        }
    }

    endwin();
    return 0;
}

board_t* init_board()
{
    board_t* b = malloc(sizeof(board_t));
    b->board_size = 9;
    b->w = 3;
    b->h = 3;
    b->state = calloc(sizeof(int), 3);
    for (int i = 0; i < 3; i++) {
        b->state[i] = calloc(sizeof(int), 3);
    }
    b->currplayer = p1;
    return b;
}

board_t* copy_board(board_t* b){
    board_t* b1 = init_board();
    b1->currplayer = b->currplayer;
    for (int i = 0; i < b->w; i++) {
        for (int j = 0; j < b->h; j++) {
            b1->state[j][i] = b->state[j][i];
        }
    }
    return b1;
}

void print_board(board_t* b, int x, int y)
{
    clear();
    printw("\n ");
    for (int k = 0; k <= 12; k++) {
        if (k % 4 == 0)
            printw("|");
        else
            printw("-");
    }
    printw("\n");
    for (int j = 0; j < b->w; j++) {
        int start = 1;
        for (int i = 0; i < b->h; i++) {
            if (start) {
                printw(" | ");
                start = 0;
            }
            if (i == x && j == y) {
                attron(COLOR_PAIR(select_color));
                printw("%s", string_opts[b->state[j][i]]);
                attron(COLOR_PAIR(normal_color));
            } else {
                printw("%s", string_opts[b->state[j][i]]);
            }
            printw(" | ");
        }
        printw("\n ");

        for (int k = 0; k <= 12; k++) {
            if (k % 4 == 0)
                printw("|");
            else
                printw("-");
        }
        printw("\n");
    }
    printw("\n press F1 to exit\n press F2 to reset");
}

void move_cursor(board_t* b, int* x, int* y)
{
    limit_cursor(x, y);
    print_board(b, *x, *y);
}

void enter_input(board_t* b, int x, int y)
{
    if (b->state[y][x] == 0) {
        b->state[y][x] = b->currplayer;
        if (b->currplayer == p1) {
            b->currplayer = p2;
        } else {
            b->currplayer = p1;
        }
        print_board(b, x, y);
        int r = test_win(b);
        if (r != 0) {
            printw("\n player %d wins!", r);
        }
    }
}

void limit_cursor(int* x, int* y)
{
    if (*x >= max_val)
        (*x)--;
    if (*y >= max_val)
        (*y)--;
    if (*x < 0)
        (*x)++;
    if (*y < 0)
        (*y)++;
}

void restart_game(board_t* b)
{
    for (int i = 0; i < b->w; i++) {
        for (int j = 0; j < b->h; j++) {
            b->state[j][i] = 0;
        }
    }
    b->currplayer = p1;
}

int test_win(board_t* b)
{
    //test vertical and horizontal
    for (int i = 0; i < b->h; i++) {
        if (b->state[i][0] == b->state[i][1] && b->state[i][0] == b->state[i][2]) {
            return b->state[i][0];
        }
        if (b->state[0][i] == b->state[1][i] && b->state[0][i] == b->state[2][i]) {
            return b->state[0][i];
        }
    }
    // test diagonals
    if (b->state[0][0] == b->state[1][1] && b->state[1][1] == b->state[2][2]) {
        return b->state[1][1];
    }
    if (b->state[0][2] == b->state[1][1] && b->state[1][1] == b->state[2][0]) {
        return b->state[1][1];
    }

    return 0;
}

int is_moves_left(board_t* b)
{
    for (int i = 0; i < b->w; i++) {
        for (int j = 0; j < b->h; j++) {
            if (b->state[j][i] != 0) {
                return 1;
            }
        }
    }
    return 0;
}

int minimax(board_t* b, int depth, int ismax)
{
    int score = test_win(b);

    if (score == p2 || score == p1) {
        return score;
    }

    if (is_moves_left(b) == 0) {
        return 0;
    }

    if (ismax) {
        int best = -100;
        for (int i = 0; i < b->w; i++) {
            for (int j = 0; j < b->h; j++) {
                if (b->state[j][i] == 0) {
                    
                }
            }
        }
    }
    return 0;
}