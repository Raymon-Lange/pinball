#ifndef _PADDLE_H
#define _PADDLE_H
#include <allegro5/allegro.h>

class paddle {
public:
    ALLEGRO_BITMAP *map;
    int x;
    int y;
    int power;
    int isGoingUp;
    int angle;
public:
    paddle(void);
    ~paddle(void);
    int load(const char *filename);
    int load(ALLEGRO_BITMAP *newMap);
};
#endif
