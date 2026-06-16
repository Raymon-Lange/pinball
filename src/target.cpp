#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "target.h"

target::target() {
    type = CIRCLE;
    xpos = 0;
    ypos = 0;
    hitState = 0;
    beingHit = false;
}

target::target(int targetType, int x, int y) {
    type = targetType;
    xpos = x;
    ypos = y;
    hitState = 0;
    beingHit = false;
}

target::~target() {}

void target::draw(ALLEGRO_BITMAP *bmp) {
    if(type == CIRCLE) {
        ALLEGRO_COLOR yellow = al_map_rgb(0, 255, 0);
        al_set_target_bitmap(bmp);
        if(hitState)
            al_draw_filled_circle(xpos, ypos, RADIUS, yellow);
        else
            al_draw_circle(xpos, ypos, RADIUS, yellow, 1);
    }
}

int target::checkHit(int x, int y) {
    int leftX   = xpos - RADIUS;
    int rightX  = xpos + RADIUS;
    int topY    = ypos - RADIUS;
    int bottomY = ypos + RADIUS;

    bool hitX = (leftX <= x && rightX >= x);
    bool hitY = (topY  <= y && bottomY >= y);

    if(hitX && hitY) {
        if(!beingHit) {
            beingHit = true;
            hitState = (hitState + 1) % 2;
            return 1;
        }
    }
    beingHit = false;
    return 0;
}
