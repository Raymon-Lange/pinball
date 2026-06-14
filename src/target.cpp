//#define CIRLCE 1
//#define RECTANGLE 2
#include "target.h"

target::target()
{
    type = CIRCLE;
    xpos = 0; 
    ypos = 0;
    hitState = 0;
    beingHit = false;
}

target::target(int targetType, int x, int y)
{
    type = targetType;
    xpos = x;
    ypos = y;
    hitState = 0;
    beingHit = false;
}

target::~target()
{
   //Do nothing atm
}

void target::draw(BITMAP *bmp)
{
    if(type == CIRCLE)
    {
        if(hitState)
        {
            circlefill(bmp, xpos, ypos, 5, YELLOW);
        }
        else
        {
            circle(bmp, xpos, ypos, 5, YELLOW);
        }
    }
    else if(type == RECTANGLE)
    {
        //TODO
    }
}

int target::checkHit(int x, int y)
{
    //step create box
    int leftX = xpos - RADIUS;
    int rightX = xpos + RADIUS;

    int topY = ypos - RADIUS;
    int bottomY = ypos + RADIUS;
    
    bool hitX = (leftX <= x && rightX >= x);
    bool hitY = (topY <= y && bottomY >= y);

    if(hitX && hitY)
    {
        if(!beingHit)
        {
            beingHit = true;
            hitState = (hitState +1) % 2;
            return 1;
        }   
    }
    beingHit = false;
    return 0;
}
