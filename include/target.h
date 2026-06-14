#ifndef _TARGET_H
#define _TARGET_H

#include <allegro.h>

#define CIRCLE 1
#define RECTANGLE 2

#define RADIUS 5

#define YELLOW makecol(0,255,0)

class target
{
private:
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
    void draw(BITMAP *bmp);
    int checkHit(int x, int y);
};
#endif
