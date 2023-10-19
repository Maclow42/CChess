#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../piece/piece.h"
#include "../coords/coords.h"
#include "../stack/stack.h"

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
    piece*** board;
    unsigned int nb_piece;
    
    piece* last_played;
    move_type last_move_type;

    coords white_king_pos;
    coords black_king_pos;

    bool white_castled;
    bool black_castled;

    stack_t* moves_stack;

    color_t to_play;
}game_board;

game_board* newBoard();
void freeBoard(game_board* board);
void initGameBoard(game_board *board);

void print_piece(char* square, piece* piece);
void printBoard(game_board* board, enum color pov, char** result);