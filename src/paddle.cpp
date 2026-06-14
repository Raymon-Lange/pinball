#include "paddle.h"

paddle::paddle(){
    map = NULL;
    x = 0;
    y = 0;
    power = 0;
    isGoingUp = 0;
    angle = 0;
}

paddle::~paddle(){
    if(map != NULL)
    {
        destroy_bitmap(map);
    }
}

int paddle::load(char *filename) {
    map = load_bitmap(filename, NULL);
    if (map == NULL)
        return 0;
    return 1;
}

int paddle::load(BITMAP *newMap)
{
    if(newMap == NULL)
        return 0;
    map = newMap;
    return 1;
}

