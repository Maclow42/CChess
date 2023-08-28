#pragma once

#include <stdlib.h>
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
    bool first_move; // usefull for pawn to double
}piece;

piece* newPiece(enum piece_type type, enum color color, unsigned int value);
piece* newPawn(enum color color);
piece* newRock(enum color color);
piece* newKnight(enum color color);
piece* newBishop(enum color color);
piece* newQueen(enum color color);
piece* newKing(enum color color);