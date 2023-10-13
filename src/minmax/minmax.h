#pragma once

#include "../gameboard/gameboard.h"
#include "../coords/coords.h"
#include "../list/list.h"
#include "../gtree/gtree.h"

movement_coords* getBestMove(game_board* board, unsigned int evaluation_depth);