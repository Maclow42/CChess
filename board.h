#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "piece.h"
#include "coords.h"

typedef enum{
    WHITE_CHESS,
    WHITE_MATE,
    WHITE_PAT,
    BLACK_CHESS,
    BLACK_MATE,
    BLACK_PAT
}game_status;

typedef struct{
    piece** board;
    coords white_king;
    coords black_king;
    game_status status;
}game_board;

game_board* newBoard();
bool isPosAccessible(game_board* board, coords* current_pos, coords* dest_pos);
bool movePiece(game_board* board, coords* current_pos, coords* dest_pos);
int isInChess(game_board* board, coords* kingpos);
bool isMate(game_board* board, coords* kingpos);
void printBoard(game_board* board);