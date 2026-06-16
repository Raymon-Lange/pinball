#include <allegro5/allegro.h>
#include <stdio.h>
#include "collisionMap.h"

collisionMap::collisionMap() {
    map = NULL;
    mapHeight = 0;
    mapWidth = 0;
}

collisionMap::~collisionMap() {
    if(map != NULL)
        al_destroy_bitmap(map);
}

int collisionMap::load(const char *filename) {
    map = al_load_bitmap(filename);
    if(map == NULL)
        return 0;
    mapWidth  = al_get_bitmap_width(map);
    mapHeight = al_get_bitmap_height(map);
    return 1;
}

int collisionMap::load(ALLEGRO_BITMAP *newMap) {
    if(newMap == NULL)
        return 0;
    map = newMap;
    mapWidth  = al_get_bitmap_width(map);
    mapHeight = al_get_bitmap_height(map);
    return 1;
}

int collisionMap::checkcollisions(int x, int y) {
    if(map == NULL) {
        printf("Error, no map loaded");
        return 0;
    }
    if(x >= mapWidth || x < 0 || y >= mapHeight || y < 0)
        return 0;

    ALLEGRO_COLOR c = al_get_pixel(map, x, y);
    unsigned char r, g, b;
    al_unmap_rgb(c, &r, &g, &b);
    return (r == 0 && g == 0 && b == 0) ? 1 : 0;
}
