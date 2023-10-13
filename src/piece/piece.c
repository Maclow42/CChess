#include "piece.h"

#include <stdlib.h>

piece* newPiece(enum piece_type type, enum color color, unsigned int value){
    piece* new_piece = malloc(sizeof(piece));
    new_piece->type = type;
    new_piece->color = color;
    new_piece->first_move = true;
    new_piece->value = value;
    return new_piece;
}

void promote_pawn(piece* pawn_piece){
    if(pawn_piece->type != PAWN)
        return;
    pawn_piece->type = QUEEN;
    pawn_piece->value = 15;
}

void unpromote_pawn(piece* queen_piece){
    if(queen_piece->type != QUEEN)
        return;
    queen_piece->type = PAWN;
    queen_piece->value = 1;
}

piece* newPawn(enum color color){
    return newPiece(PAWN, color, 1);
}

piece* newRock(enum color color){
    return newPiece(ROCK, color, 5);
}

piece* newKnight(enum color color){
    return newPiece(KNIGHT, color, 3);
}

piece* newBishop(enum color color){
    return newPiece(BISHOP, color, 3);
}

piece* newQueen(enum color color){
    return newPiece(QUEEN, color, 15);
}

piece* newKing(enum color color){
    return newPiece(KING, color, 0);
}