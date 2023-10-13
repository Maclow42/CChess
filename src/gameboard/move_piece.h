#pragma once

#include <stdbool.h>

#include "gameboard.h"
#include "../piece/piece.h"
#include "../coords/coords.h"
#include "../list/list.h"

bool playerMovePiece(game_board* board, coords current_pos, coords dest_pos);
piece* movePiece(game_board* board, coords current_pos, coords dest_pos);

list_t* getAccessiblePos(game_board* board, enum color color);