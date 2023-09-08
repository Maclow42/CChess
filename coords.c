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

bool areMovementCoordsEqual(movement_coords* move1, movement_coords* move2){
    return areCoordsEqual(move1->start_pos, move2->start_pos) && areCoordsEqual(move1->end_pos, move2->end_pos);
}

coords* Coords(int posx, int posy){
    coords* result = malloc(sizeof(coords));
    result->posx = posx;
    result->posy = posy;
    return result;
}