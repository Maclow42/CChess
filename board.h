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
    BLACK_PAT,
    RAS
}game_status;

typedef struct{
    piece** board;
    coords* white_king_pos;
    coords* black_king_pos;
    game_status status;
    color_t to_play;
}game_board;

game_board* newBoard();
bool isPosAccessible(game_board* board, coords* current_pos, coords* dest_pos);
bool playerMovePiece(game_board* board, coords* current_pos, coords* dest_pos);

int isInChess(game_board* board, coords* kingpos);
bool isMate(game_board* board, coords* kingpos);
game_status getGameStatus(game_board* board);

void printBoard(game_board* board);