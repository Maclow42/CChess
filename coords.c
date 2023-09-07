#include <stdlib.h>
#include <stdio.h>
#include "coords.h"

movement_coords* moveCoords(coords start, coords end, int score){
    /*
        * Create a movement_coords struct
        * @param start : the starting position of the piece
        * @param end : the ending position of the piece
    */
    movement_coords* result = malloc(sizeof(movement_coords));
    result->start_pos = (coords) {start.posx, start.posy};
    result->end_pos = (coords) {end.posx, end.posy};
    result->score = score;
    return result;
}

void print_MoveCoords(movement_coords* move){
    printf("%c%c -> %c%c\n", move->start_pos.posx + 'A', move->start_pos.posy + '1', move->end_pos.posx + 'A', move->end_pos.posy + '1');
}