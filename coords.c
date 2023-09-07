#include <stdio.h>
#include "coords.h"

void print_MoveCoords(movement_coords* move){
    printf("%c%c -> %c%c\n", move->start_pos.posx + 'A', move->start_pos.posy + '1', move->end_pos.posx + 'A', move->end_pos.posy + '1');
}