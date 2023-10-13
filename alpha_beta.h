#pragma once

#include "board.h"
#include "coords.h"
#include "./src/list/list.h"
#include "./src/gtree/gtree.h"

movement_coords* getBestMove(game_board* board, unsigned int evaluation_depth);