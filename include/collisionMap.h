#ifndef _COLLISIONMAP_H
#define _COLLISIONMAP_H
#include "allegro.h"
#include <iostream>
#include <string>


#define BLACK makecol(0,0,0);

using namespace std;

class collisionMap{
private:
public:
	BITMAP *map;
	int mapHeight;
	int mapWidth;
public:
	collisionMap(void);
	~collisionMap(void);
	int load(char *filename);
	int load(BITMAP *newMap);
	int checkcollisions(int x, int y);
};

#endif
