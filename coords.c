#include <stdlib.h>
#include "coords.h"

coords* Coords(int posx, int posy){
    coords* result = malloc(sizeof(coords));
    result->posx = posx;
    result->posy = posy;
    return result;
}