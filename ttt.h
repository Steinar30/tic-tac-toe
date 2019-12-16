#define max_val 3
#define normal_color 1
#define select_color 2
#define p1 1
#define p2 2

typedef struct board board_t;
struct board {
    int board_size;
    int w, h;
    int currplayer;
    int** state;
    int moves;
};

board_t* init_board();
void restart_game(board_t* b);
void print_board(board_t* b, int x, int y);
void move_cursor(board_t* b, int* x, int* y);
void limit_cursor(int* x, int* y);
void enter_input(board_t* b, int x, int y);
int test_win(board_t* b);
int is_moves_left(board_t* b);
void do_best_move(board_t* b);