#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "paddle.h"

paddle::paddle() {
    map = NULL;
    x = 0; y = 0;
    power = 0;
    isGoingUp = 0;
    angle = 0;
}

paddle::~paddle() {
    if(map != NULL)
        al_destroy_bitmap(map);
}

int paddle::load(const char *filename) {
    map = al_load_bitmap(filename);
    if(map == NULL)
        return 0;
    // black (index 0 in original 8-bit sprite) was the transparent background
    al_convert_mask_to_alpha(map, al_map_rgb(255, 0, 255));
    return 1;
}

int paddle::load(ALLEGRO_BITMAP *newMap) {
    if(newMap == NULL)
        return 0;
    map = newMap;
    return 1;
}
