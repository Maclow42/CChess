#pragma once

#include "board.h"
#include "coords.h"
#include "./src/list/list.h"
#include "./src/tree/tree.h"
#include "./src/gtree/gtree.h"

typedef struct{
    game_board *board;

    unsigned int evaluation_depth;
    gtree_t* treeEvaluation;
}alpha_beta_predictor;

alpha_beta_predictor* new_Alpha_Beta_Predictor(game_board* board, unsigned int evaluation_depth);
void free_Alpha_Beta_Predictor(alpha_beta_predictor* predictor);

movement_coords* getBestMove(alpha_beta_predictor *predictor);

void predictor_update_with_move(alpha_beta_predictor* predictor, movement_coords* move);