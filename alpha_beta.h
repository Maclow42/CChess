#pragma once

#include "board.h"
#include "coords.h"
#include "./src/list/list.h"
#include "./src/tree/tree.h"

typedef struct{
    coords start_pos;
    coords end_pos;
    int score;
}movement_coords;

int evaluateBoard(game_board* board);
list_t* getAllPossibleMove(game_board* board, enum color color);

movement_coords* getBestMove(game_board* board, unsigned int evaluation_depth);