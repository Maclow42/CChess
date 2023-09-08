#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "piece.h"
#include "coords.h"
#include "./src/list/list.h"
#include "./src/stack/stack.h"

typedef enum{
    WHITE_CHESS,
    WHITE_MATE,
    BLACK_CHESS,
    BLACK_MATE,
    PAT,
    RAS
}game_status;

typedef enum{
    NORMAL_MOVE,
    PAWN_PROMOTION,
    LITTLE_CASTLE,
    BIG_CASTLE,
}move_type;

typedef struct{
    coords start_pos;
    coords end_pos;
    piece* taken_piece;
    move_type last_move;
}state_backuper;

typedef struct{
    piece*** board;
    unsigned int nb_piece;
    
    piece* last_played;
    move_type last_move_type;

    coords white_king_pos;
    coords black_king_pos;

    game_status status;
    bool white_castled;
    bool black_castled;

    stack_t* moves_stack;

    color_t to_play;
}game_board;

game_board* newBoard();
void freeBoard(game_board* board);
void initGameBoard(game_board *board);

bool isPosAccessible(game_board* board, coords current_pos, coords dest_pos);
bool playerMovePiece(game_board* board, coords current_pos, coords dest_pos);
piece* movePiece(game_board* board, coords current_pos, coords dest_pos);

bool isInChess(game_board* board, enum color color);
bool isMate(game_board* board, enum color color);
game_status getGameStatus(game_board* board);

list_t* getAllPossibleMoves(game_board* board, enum color color);

void printBoard(game_board* board, enum color pov);

void backup_state(game_board* board, coords start_pos, coords end_pos);
void restore_state(game_board* board);



#define DB printf("%s:%d\n", __FILE__, __LINE__);