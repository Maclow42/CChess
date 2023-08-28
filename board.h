#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "piece.h"
#include "coords.h"
#include "./src/list/list.h"

typedef enum{
    WHITE_CHESS,
    WHITE_MATE,
    BLACK_CHESS,
    BLACK_MATE,
    PAT,
    RAS
}game_status;

typedef struct{
    piece*** board;
    piece* last_played;
    coords white_king_pos;
    coords black_king_pos;
    game_status status;
    color_t to_play;
}game_board;

game_board* newBoard();
void freeBoard(game_board* board);
void initGameBoard(game_board *board);

bool isPosAccessible(game_board* board, coords current_pos, coords dest_pos);
bool playerMovePiece(game_board* board, coords current_pos, coords dest_pos);
bool movePiece(game_board* board, coords current_pos, coords dest_pos, bool force);

int isInChess(game_board* board, coords kingpos);
bool isMate(game_board* board, coords kingpos);
game_status getGameStatus(game_board* board);

list_t* getPossiblePos(game_board* board, coords current_pos);

void printBoard(game_board* board);