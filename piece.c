#include "piece.h"

piece newPiece(enum piece_type type, enum color color){
    piece new_piece;
    new_piece.type = type;
    new_piece.color = color;
    new_piece.first_move = true;
    return new_piece;
}