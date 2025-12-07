#include <stdio.h>
#include <stdlib.h>
#include "chess.h"

// =========================
// 1) INITIALIZATION
// =========================

void init_board(Board *b) {
    // Black pieces (row 8)
    b->squares[0][0] = 'r'; b->squares[0][1] = 'n';
    b->squares[0][2] = 'b'; b->squares[0][3] = 'q';
    b->squares[0][4] = 'k'; b->squares[0][5] = 'b';
    b->squares[0][6] = 'n'; b->squares[0][7] = 'r';

    // Black pawns
    for (int j = 0; j < SIZE; j++)
        b->squares[1][j] = 'p';

    // Empty squares
    for (int i = 2; i <= 5; i++)
        for (int j = 0; j < SIZE; j++)
            b->squares[i][j] = '.';

    // White pawns
    for (int j = 0; j < SIZE; j++)
        b->squares[6][j] = 'P';

    // White pieces (row 1)
    b->squares[7][0] = 'R'; b->squares[7][1] = 'N';
    b->squares[7][2] = 'B'; b->squares[7][3] = 'Q';
    b->squares[7][4] = 'K'; b->squares[7][5] = 'B';
    b->squares[7][6] = 'N'; b->squares[7][7] = 'R';
}

void init_game(Game *g) {
    init_board(&g->board);
    g->white_to_play = 1;
    g->move_count = 0;
}

// =========================
// 2) DISPLAY
// =========================

void print_board(const Board *b) {
    printf("  a b c d e f g h\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < SIZE; j++)
            printf("%c ", b->squares[i][j]);
        printf("%d\n", 8 - i);
    }
    printf("  a b c d e f g h\n");
}

// =========================
// 3) COORD HANDLING
// =========================

int coords_valid(char col, int row) {
    return (col >= 'a' && col <= 'h' && row >= 1 && row <= 8);
}

void coord_to_index(char col, int row, int *i, int *j) {
    *j = col - 'a';
    *i = 8 - row;
}

// =========================
// 4) UTILITIES
// =========================

int is_white_piece(char p) { return (p >= 'A' && p <= 'Z'); }
int is_black_piece(char p) { return (p >= 'a' && p <= 'z'); }
int is_empty(char p)       { return p == '.'; }

static int same_color(char a, char b) {
    if (is_empty(a) || is_empty(b)) return 0;
    return (is_white_piece(a) && is_white_piece(b)) ||
           (is_black_piece(a) && is_black_piece(b));
}

// =========================
// 5) PATH CHECKING
// =========================

static int path_clear_line(const Board *b,
                           int r1, int c1, int r2, int c2) {
    if (r1 == r2) {
        int step = (c2 > c1) ? 1 : -1;
        for (int c = c1 + step; c != c2; c += step)
            if (!is_empty(b->squares[r1][c])) return 0;
        return 1;
    }
    if (c1 == c2) {
        int step = (r2 > r1) ? 1 : -1;
        for (int r = r1 + step; r != r2; r += step)
            if (!is_empty(b->squares[r][c1])) return 0;
        return 1;
    }
    return 0;
}

static int path_clear_diag(const Board *b,
                           int r1, int c1, int r2, int c2) {
    int dr = r2 - r1, dc = c2 - c1;
    if (abs(dr) != abs(dc)) return 0;
    int sr = (dr > 0) ? 1 : -1;
    int sc = (dc > 0) ? 1 : -1;
    int r = r1 + sr, c = c1 + sc;
    while (r != r2 && c != c2) {
        if (!is_empty(b->squares[r][c])) return 0;
        r += sr; c += sc;
    }
    return 1;
}

// =========================
// 6) ATTACK CHECKING
// =========================

static int piece_attacks(const Board *b,
                         int r1, int c1,
                         int r2, int c2) {
    char p = b->squares[r1][c1];
    int dr = r2 - r1, dc = c2 - c1;

    switch (p) {
    case 'P': return (dr == -1 && abs(dc) == 1);
    case 'p': return (dr == 1  && abs(dc) == 1);

    case 'R': case 'r':
        return (r1 == r2 || c1 == c2) &&
               path_clear_line(b, r1, c1, r2, c2);

    case 'B': case 'b':
        return path_clear_diag(b, r1, c1, r2, c2);

    case 'Q': case 'q':
        if (r1 == r2 || c1 == c2)
            return path_clear_line(b, r1, c1, r2, c2);
        return path_clear_diag(b, r1, c1, r2, c2);

    case 'N': case 'n':
        return (abs(dr) == 2 && abs(dc) == 1) ||
               (abs(dr) == 1 && abs(dc) == 2);

    case 'K': case 'k':
        return abs(dr) <= 1 && abs(dc) <= 1;

    default: return 0;
    }
}

int is_in_check(const Game *g, int white) {
    const Board *b = &g->board;
    char king = white ? 'K' : 'k';

    int kr = -1, kc = -1;
    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++)
            if (b->squares[r][c] == king) {
                kr = r; kc = c;
            }

    for (int r = 0; r < SIZE; r++)
        for (int c = 0; c < SIZE; c++) {
            char p = b->squares[r][c];
            if (white && is_white_piece(p)) continue;
            if (!white && is_black_piece(p)) continue;
            if (piece_attacks(b, r, c, kr, kc)) return 1;
        }

    return 0;
}

// =========================
// 7) MOVE LEGALITY
// =========================

int move_is_legal(const Game *g,
                  int r1, int c1,
                  int r2, int c2) {

    const Board *b = &g->board;
    char p = b->squares[r1][c1];
    char dest = b->squares[r2][c2];

    if (is_empty(p)) return 0;
    if (same_color(p, dest)) return 0;

    int white = g->white_to_play;
    if (white && !is_white_piece(p)) return 0;
    if (!white && !is_black_piece(p)) return 0;

    int dr = r2 - r1, dc = c2 - c1;
    int basic_ok = 0;

    switch (p) {
    case 'P':
        if (dc == 0 && dr == -1 && is_empty(dest)) basic_ok = 1;
        else if (dc == 0 && dr == -2 && r1 == 6 &&
                 is_empty(dest) && is_empty(b->squares[r1-1][c1])) basic_ok = 1;
        else if (dr == -1 && abs(dc) == 1 && is_black_piece(dest)) basic_ok = 1;
        break;

    case 'p':
        if (dc == 0 && dr == 1 && is_empty(dest)) basic_ok = 1;
        else if (dc == 0 && dr == 2 && r1 == 1 &&
                 is_empty(dest) && is_empty(b->squares[r1+1][c1])) basic_ok = 1;
        else if (dr == 1 && abs(dc) == 1 && is_white_piece(dest)) basic_ok = 1;
        break;

    case 'R': case 'r':
        if ((r1 == r2 || c1 == c2) && path_clear_line(b, r1, c1, r2, c2))
            basic_ok = 1;
        break;

    case 'B': case 'b':
        if (path_clear_diag(b, r1, c1, r2, c2)) basic_ok = 1;
        break;

    case 'Q': case 'q':
        if (r1 == r2 || c1 == c2) {
            if (path_clear_line(b, r1, c1, r2, c2)) basic_ok = 1;
        } else {
            if (path_clear_diag(b, r1, c1, r2, c2)) basic_ok = 1;
        }
        break;

    case 'N': case 'n':
        if ((abs(dr) == 2 && abs(dc) == 1) ||
            (abs(dr) == 1 && abs(dc) == 2))
            basic_ok = 1;
        break;

    case 'K': case 'k':
        if (abs(dr) <= 1 && abs(dc) <= 1)
            basic_ok = 1;
        break;
    }

    if (!basic_ok) return 0;

    // Simulate move and check king safety
    Game temp = *g;
    temp.board.squares[r2][c2] = p;
    temp.board.squares[r1][c1] = '.';

    int moving_is_white = is_white_piece(p);
    if (is_in_check(&temp, moving_is_white)) return 0;

    return 1;
}

// =========================
// 8) APPLY MOVE
// =========================

void apply_move(Game *g,
                int r1, int c1,
                int r2, int c2) {

    char p = g->board.squares[r1][c1];

    g->board.squares[r2][c2] = p;
    g->board.squares[r1][c1] = '.';

    // Promotion
    if (p == 'P' && r2 == 0) g->board.squares[r2][c2] = 'Q';
    if (p == 'p' && r2 == 7) g->board.squares[r2][c2] = 'q';

    g->move_count++;
}

// =========================
// 9) CHECKMATE / STALEMATE
// =========================

int has_any_legal_move(const Game *g, int white) {
    Game temp = *g;
    temp.white_to_play = white;

    for (int r1 = 0; r1 < SIZE; r1++)
        for (int c1 = 0; c1 < SIZE; c1++) {
            char p = temp.board.squares[r1][c1];
            if (white && !is_white_piece(p)) continue;
            if (!white && !is_black_piece(p)) continue;
            if (is_empty(p)) continue;

            for (int r2 = 0; r2 < SIZE; r2++)
                for (int c2 = 0; c2 < SIZE; c2++)
                    if (move_is_legal(&temp, r1, c1, r2, c2))
                        return 1;
        }
    return 0;
}

int is_checkmate(const Game *g, int white) {
    return is_in_check(g, white) && !has_any_legal_move(g, white);
}

// =========================
// 10) GAME LOOP
// =========================

static int play_one_turn(Game *g) {
    char orig[3], dest[3];

    print_board(&g->board);
    printf("\nMove %d - Player: %s\n",
           g->move_count + 1,
           g->white_to_play ? "White" : "Black");

    if (is_in_check(g, g->white_to_play))
        printf("WARNING: You are in CHECK!\n");

    printf("From (ex: e2, q to quit): ");
    if (scanf("%2s", orig) != 1) return 0;
    if (orig[0] == 'q') return 0;

    printf("To (ex: e4): ");
    if (scanf("%2s", dest) != 1) return 0;

    char co = orig[0];
    int ro = orig[1] - '0';
    char cd = dest[0];
    int rd = dest[1] - '0';

    if (!coords_valid(co, ro) || !coords_valid(cd, rd)) {
        printf("Invalid coordinates.\n");
        return 1;
    }

    int r1, c1, r2, c2;
    coord_to_index(co, ro, &r1, &c1);
    coord_to_index(cd, rd, &r2, &c2);

    if (!move_is_legal(g, r1, c1, r2, c2)) {
        printf("Illegal move.\n");
        return 1;
    }

    apply_move(g, r1, c1, r2, c2);

    int opponent = !g->white_to_play;

    if (is_checkmate(g, opponent)) {
        print_board(&g->board);
        printf("\nCHECKMATE! %s wins.\n",
               g->white_to_play ? "White" : "Black");
        return 0;
    }

    if (!is_in_check(g, opponent) &&
        !has_any_legal_move(g, opponent)) {
        print_board(&g->board);
        printf("\nSTALEMATE! Draw.\n");
        return 0;
    }

    g->white_to_play = opponent;
    return 1;
}

void play_game(Game *g) {
    while (play_one_turn(g));
    printf("Game over.\n");
}
