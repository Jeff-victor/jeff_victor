#ifndef SAVE_H
#define SAVE_H

#include "chess.h"

// Save game to text file
int save_game(const Game *g, const char *filename);

// Load game from text file
int load_game(Game *g, const char *filename);

#endif
