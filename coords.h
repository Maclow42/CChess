#pragma once

typedef struct{
    unsigned int posx;
    unsigned int posy;
}coords;

typedef struct{
    coords start_pos;
    coords end_pos;
    int score;
}movement_coords;

#define Coords(posx, posy) ((coords){posx, posy})

#define areCoordsValid(coords) ((coords.posx) < 8 && (coords.posy) < 8)

void print_MoveCoords(movement_coords* move);