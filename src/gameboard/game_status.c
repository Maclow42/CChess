#include "game_status.h"
#include "accessible_pos.h"

game_status getGameStatus(game_board* board)
{
    if(board->nb_piece == 2)
        return PAT;

    bool white_chess = isInChess(board, WHITE);
    bool white_mate = isMate(board, WHITE);

    if(white_chess){
        if(white_mate)
            return WHITE_MATE;
        return WHITE_CHESS;
    }
    if(white_mate)
        return PAT;

    bool black_chess = isInChess(board, BLACK);
    bool black_mate = isMate(board, BLACK);

    if(black_chess){
        if(black_mate)
            return BLACK_MATE;
        return BLACK_CHESS;
    }
    if(black_mate)
        return PAT;

    return RAS;
}