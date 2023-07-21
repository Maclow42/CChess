#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef enum color {
    NONE_COLOR,
    BLACK,
    WHITE
};

enum piece_type {
    NONE_PIECE,
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
    bool first_move; // usefull for pawn to double
}piece;

piece newPiece(enum piece_type type, enum color color);