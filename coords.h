#pragma once

#include <stdbool.h>

typedef struct{
    unsigned int posx;
    unsigned int posy;
}coords;

typedef struct{
    coords start_pos;
    coords end_pos;
    int score;
}movement_coords;

coords Coords(unsigned int posx, unsigned int posy);
bool areCoordsValid(coords* coords);

void print_MoveCoords(movement_coords* move);