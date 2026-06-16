#ifndef _PINBALL_H
#define _PINBALL_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "collisionMap.h"
#include "paddle.h"
#include "targetHandler.h"

#define WIDTH  400
#define HEIGHT 680

struct ball {
    double x, y;
    double a, b;
    double ang, angN, mag;
    double angX, angY, magG;
    int numAng;
} pinballs[2];

int             gameover = 0;
ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP  *background;
ALLEGRO_BITMAP  *paddleBuffer;

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

#endif
