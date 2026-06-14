#include "targetHandler.h"

targetHandler::targetHandler(void)
{
    count = 0;
}

targetHandler::~targetHandler()
{
    //delete the sprites
    for (int n = 0; n < count; n++)
        delete targets[n];
}

void targetHandler::add(target *tgr)
{
    if (tgr != NULL) {
        targets[count] = tgr;
        count++;
    }
}

void targetHandler::create(int xpos, int ypos, int type)
{
    targets[count] = new target(type, xpos, ypos);
    count++;
}

void targetHandler::removeAll()
{
    //delete the sprites
    for (int n = 0; n < count; n++)
         delete targets[n];

    count = 0;
}

void targetHandler::checkHit(int xpos, int ypos)
{
    for (int n = 0; n < count; n++)
        targets[n]->checkHit(xpos, ypos);
}

void targetHandler::draw(BITMAP *bmp)
{
    for (int n = 0; n < count; n++)
    {
         if(targets[n] != NULL)
         {
             targets[n]->draw(bmp);
         }
    }
}

