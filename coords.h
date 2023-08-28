#pragma once

#include <stdbool.h>

typedef struct{
    unsigned int posx;
    unsigned int posy;
}coords;

coords Coords(unsigned int posx, unsigned int posy);
bool areCoordsValid(coords* coords);