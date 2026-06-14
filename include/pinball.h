// Author: Raymon Lange
// Pinball.h this is my 1st pin ball game
#ifndef _PINBALL_H
#define _PINBALL_H

#include "allegro.h"
#include "collisionMap.h"
#include "paddle.h"
#include "targetHandler.h"

#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 400 
#define HEIGHT 680
#define RED makecol(255,0,0)

struct ball
{
    double x;
    double y;
    double a;
    double b;
    double ang;
    double angN;
    double mag;
    double angX;
    double angY;
    double magG;
    int numAng;
}pinballs[2];


int gameover = 0;
BITMAP *background;
BITMAP *paddleBuffer;
 
collisionMap *map;
collisionMap *paddleMap;
collisionMap *leftBummer;
collisionMap *rightBummer;
collisionMap *lowBummer;
collisionMap *leftTriBummer;
collisionMap *rightTriBummer;

paddle *leftPaddle;
paddle *rightPaddle;

targetHandler *targets;

void initialize();
void initializeBackground();
#endif
