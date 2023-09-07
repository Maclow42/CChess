#pragma once

#include "board.h"
#include "coords.h"
#include "./src/list/list.h"
#include "./src/tree/tree.h"

typedef struct{
    game_board *board;

    unsigned int evaluation_depth;
    tree_t* treeEvaluation;
}alpha_beta_predictor;

alpha_beta_predictor* new_Alpha_Beta_Predictor(game_board* board, unsigned int evaluation_depth);
void free_Alpha_Beta_Predictor(alpha_beta_predictor* predictor);

movement_coords* getBestMove(alpha_beta_predictor *predictor);