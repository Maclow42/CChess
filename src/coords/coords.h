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

#define areCoordsValid(coords) ((coords.posx) < 8 && (coords.posy) < 8)

#define areCoordsEqual(coords1, coords2) ((coords1.posx) == (coords2.posx) && (coords1.posy) == (coords2.posy))

bool areMovementCoordsEqual(movement_coords* move1, movement_coords* move2);

movement_coords* moveCoords(coords start, coords end, int score);

coords* Coords(int posx, int posy);

void print_MoveCoords(movement_coords* move);