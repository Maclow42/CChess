#include <stdlib.h>
#include "coords.h"

coords Coords(unsigned int posx, unsigned int posy){
    coords result = {posx, posy};
    return result;
}

bool areCoordsValid(coords* coords){
    return coords->posx < 8 && coords->posy < 8;
}