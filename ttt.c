#include "ttt.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

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
            x = 0, y = 0;
            print_board(board, x, y);
            break;
        case KEY_F(3):
            board->computer_playing = !board->computer_playing;
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
    b->w = max_val;
    b->h = max_val;
    b->moves = 0;
    b->state = calloc(sizeof(int), 3);
    for (int i = 0; i < 3; i++) {
        b->state[i] = calloc(sizeof(int), 3);
    }
    b->currplayer = p1;
    b->computer_playing = 1;
    b->p1_session_score = 0;
    b->p2_session_score = 0;
    b->comp_session_score = 0;
    b->game_in_progress = 1;
    return b;
}

board_t* copy_board(board_t* b)
{
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

    if (b->computer_playing) {
        printw("\n player score %d computer score %d\n", b->p1_comp_session_score, b->comp_session_score);
    } else {
        printw("\n p1 score %d p2 score %d\n", b->p1_session_score, b->p2_session_score);
    }

    printw("\n press F1 to exit\n press F2 to reset\n press F3 to toggle computer");
}

void move_cursor(board_t* b, int* x, int* y)
{
    if (b->game_in_progress) {
        move_cursor_safe(x, y);
        print_board(b, *x, *y);
    }
}

void do_move(board_t* b, int x, int y)
{
    b->state[y][x] = b->currplayer;
    if (b->currplayer == p1) {
        b->currplayer = p2;
    } else {
        b->currplayer = p1;
    }
    b->moves++;
}

int test_end_game(board_t* b, int x, int y)
{
    int r = test_win(b);
    if (r != 0) {
        if (r == p1) {
            if (b->computer_playing) {
                b->p1_comp_session_score++;
            } else {
                b->p1_session_score++;
            }
        } else if (r == p2) {
            if (b->computer_playing) {
                b->comp_session_score++;
            } else {
                b->p2_session_score++;
            }
        }
        b->game_in_progress = 0;
        print_board(b, x, y);
        printw("\n player %d wins!", r);
        return 1;
    }
    if (b->moves >= b->h * b->w) {
        printw("\n its a draw\n");
        b->game_in_progress = 0;
        return 1;
    }
    print_board(b, x, y);
    return 0;
}

void enter_input(board_t* b, int x, int y)
{
    if (b->state[y][x] == 0 && b->game_in_progress) {
        do_move(b, x, y);
        if (test_end_game(b, x, y))
            return;
        if (b->computer_playing) {
            do_best_move(b);
            test_end_game(b, x, y);
        }
    }
}

void move_cursor_safe(int* x, int* y)
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
    b->game_in_progress = 1;
    b->moves = 0;
}

int test_win(board_t* b)
{
    //test vertical and horizontal
    for (int i = 0; i < b->h; i++) {
        if (b->state[i][0] == b->state[i][1] && b->state[i][1] == b->state[i][2] && b->state[i][0] != 0) {
            return b->state[i][0];
        }
        if (b->state[0][i] == b->state[1][i] && b->state[1][i] == b->state[2][i] && b->state[0][i] != 0) {
            return b->state[0][i];
        }
    }
    // test diagonals
    if (b->state[0][0] == b->state[1][1] && b->state[1][1] == b->state[2][2] && b->state[1][1] != 0) {
        return b->state[1][1];
    }
    if (b->state[0][2] == b->state[1][1] && b->state[1][1] == b->state[2][0] && b->state[1][1] != 0) {
        return b->state[1][1];
    }

    return 0;
}

void do_best_move(board_t* b)
{
    //either place in centre or in corner
    if (b->moves == 1) {
        if (b->state[1][1] == p1) {
            do_move(b, 0, 0);
        } else {
            do_move(b, 1, 1);
        }
        return;
    }

    //test if any move wins for p2
    for (int i = 0; i < b->w; i++) {
        for (int j = 0; j < b->h; j++) {
            if (b->state[j][i] == 0) {
                b->state[j][i] = p2;
                int win = test_win(b);
                b->state[j][i] = 0;
                if (win == p2) {
                    do_move(b, i, j);
                    return;
                }
            }
        }
    }

    //test if any move lets p1 win, if so do said move
    for (int i = 0; i < b->w; i++) {
        for (int j = 0; j < b->h; j++) {
            if (b->state[j][i] == 0) {
                b->state[j][i] = p1;
                int win = test_win(b);
                b->state[j][i] = 0;
                if (win == p1) {
                    do_move(b, i, j);
                    return;
                }
            }
        }
    }

    //pick first empty spot if p1 cannot win atm
    for (int i = 0; i < b->w; i++) {
        for (int j = 0; j < b->h; j++) {
            if (b->state[j][i] == 0) {
                do_move(b, i, j);
                return;
            }
        }
    }
}