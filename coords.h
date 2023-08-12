#pragma once

#include <stdbool.h>

typedef struct{
    unsigned int posx;
    unsigned int posy;
}coords;

coords* Coords(int posx, int posy);
bool areCoordsValid(coords* coords);