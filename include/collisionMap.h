#ifndef _COLLISIONMAP_H
#define _COLLISIONMAP_H
#include <allegro5/allegro.h>

class collisionMap {
public:
    ALLEGRO_BITMAP *map;
    int mapHeight;
    int mapWidth;
public:
    collisionMap(void);
    ~collisionMap(void);
    int load(const char *filename);
    int load(ALLEGRO_BITMAP *newMap);
    int checkcollisions(int x, int y);
};

#endif
