#include <stdio.h>
#include "save.h"

int save_game(const Game *g, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;

    fprintf(f, "%d %d\n", g->white_to_play, g->move_count);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            fputc(g->board.squares[i][j], f);
        fputc('\n', f);
    }

    fclose(f);
    return 1;
}

int load_game(Game *g, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    if (fscanf(f, "%d %d\n", &g->white_to_play, &g->move_count) != 2) {
        fclose(f);
        return 0;
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int c = fgetc(f);
            if (c == EOF || c == '\n') {
                fclose(f);
                return 0;
            }
            g->board.squares[i][j] = (char)c;
        }
        fgetc(f);
    }

    fclose(f);
    return 1;
}

