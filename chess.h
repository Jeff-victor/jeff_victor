#ifndef CHESS_H
#define CHESS_H

#define SIZE 8

// Represents the chess board
typedef struct {
    char squares[SIZE][SIZE];
} Board;

// Represents the state of the game
typedef struct {
    Board board;
    int white_to_play;   // 1 = white's turn, 0 = black's turn
    int move_count;
} Game;

// ===== Basic functions =====

// Initialize game and board
void init_game(Game *g);

// Display the board
void print_board(const Board *b);

// Check if coordinates like 'e',2 are valid
int coords_valid(char col, int row);

// Convert chess coords 'e',2 to array indices i,j
void coord_to_index(char col, int row, int *i, int *j);

// ===== Piece utilities =====
int is_white_piece(char p);
int is_black_piece(char p);
int is_empty(char p);

// ===== Move legality =====
int move_is_legal(const Game *g,
                  int from_row, int from_col,
                  int to_row, int to_col);

// Apply a move (assumed legal)
void apply_move(Game *g,
                int from_row, int from_col,
                int to_row, int to_col);

// ===== Check / Checkmate =====

// Returns 1 if white/black king is in check
int is_in_check(const Game *g, int white);

// Returns 1 if this side has any legal move
int has_any_legal_move(const Game *g, int white);

// Returns 1 if this side is in checkmate
int is_checkmate(const Game *g, int white);

// ===== Game loop =====
void play_game(Game *g);

#endif
