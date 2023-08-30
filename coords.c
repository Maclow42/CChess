#include <stdio.h>
#include "coords.h"

coords Coords(unsigned int posx, unsigned int posy){
    coords result = {posx, posy};
    return result;
}

bool areCoordsValid(coords* coords){
    return coords->posx < 8 && coords->posy < 8;
}

void print_MoveCoords(movement_coords* move){
    printf("%c%c -> %c%c\n", move->start_pos.posx + 'A', move->start_pos.posy + '1', move->end_pos.posx + 'A', move->end_pos.posy + '1');
}