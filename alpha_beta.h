#pragma once

#include "board.h"
#include "coords.h"
#include "./src/list/list.h"
#include "./src/tree/tree.h"

int evaluateBoard(game_board* board);
list_t* getAllPossibleMove(game_board* board, enum color color);

movement_coords* getBestMove(game_board* board, unsigned int evaluation_depth);