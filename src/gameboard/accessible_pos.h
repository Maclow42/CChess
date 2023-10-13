#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gameboard.h"

#include "../coords/coords.h"
#include "../list/list.h"


bool isPosAccessible_Pawn(game_board* game_board, coords current_pos, coords end_pos);
bool isPosAccessible_Rock(game_board* game_board, coords current_pos, coords end_pos);
bool isPosAccessible_Knight(game_board* game_board, coords current_pos, coords end_pos);
bool isPosAccessible_Bishop(game_board* game_board, coords current_pos, coords end_pos);
bool isPosAccessible_Queen(game_board* game_board, coords current_pos, coords end_pos);
bool isPosAccessible_King(game_board* game_board, coords current_pos, coords end_pos);
bool isPosAccessible(game_board* board, coords current_pos, coords dest_pos);


list_t* getAccessiblePos(game_board* board, enum color color);


bool isInChess(game_board* board, enum color color);
bool isMate(game_board* board, enum color color);

