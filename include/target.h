#ifndef _TARGET_H
#define _TARGET_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define CIRCLE    1
#define RECTANGLE 2
#define RADIUS    5

class target {
public:
    int type;
    int xpos;
    int ypos;
    int hitState;
    bool beingHit;
public:
    target(void);
    target(int type, int xpos, int ypos);
    ~target(void);
    void draw(ALLEGRO_BITMAP *bmp);
    int checkHit(int x, int y);
};
#endif
