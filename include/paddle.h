#ifndef _PADDLE_h
#define _PADDLE_h
#include "allegro.h"
#include <string>

using namespace std;

class paddle{
private:
public:
    BITMAP *map;
    int x;
    int y;
    int power;
    int isGoingUp;
    int angle;
public:
    paddle(void);
    ~paddle(void);
    int load(char *filename);
    int load(BITMAP *newMap);
};
#endif
