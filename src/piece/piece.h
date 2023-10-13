#pragma once

#include <stdbool.h>

typedef enum color {
    NONE_COLOR,
    BLACK,
    WHITE
}color_t;

enum piece_type {
    PAWN,
    ROCK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

typedef struct{
    enum piece_type type;
    enum color color;

    unsigned int value;
    bool first_move; // usefull for pawn to double/en passant and for king/rock to castle

}piece;

piece* newPiece(enum piece_type type, enum color color, unsigned int value);
piece* newPawn(enum color color);
piece* newRock(enum color color);
piece* newKnight(enum color color);
piece* newBishop(enum color color);
piece* newQueen(enum color color);
piece* newKing(enum color color);

void promote_pawn(piece* pawn_piece);
void unpromote_pawn(piece* queen_piece);