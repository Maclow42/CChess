#pragma once

#include <stdbool.h>

#include "gameboard.h"
#include "../piece/piece.h"
#include "../coords/coords.h"
#include "../list/list.h"

typedef enum{
    ERROR_NO_PIECE_AT_GIVEN_POS,
    ERROR_INVALID_MOVE,
    ERROR_CHESS,
    ERROR_WRONG_COLOR,
    OK_DONE
}move_return_code;

move_return_code playerMovePiece(game_board* board, coords current_pos, coords dest_pos);
piece* movePiece(game_board* board, coords current_pos, coords dest_pos);

list_t* getAccessiblePos(game_board* board, enum color color);