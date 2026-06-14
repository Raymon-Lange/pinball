#include <allegro.h>
#include "collisionMap.h"
#include "math.h"

collisionMap::collisionMap(){
	map = NULL;
	mapHeight = 0;
	mapWidth = 0;
}

collisionMap::~collisionMap(){
    if(map != NULL)
    {
        destroy_bitmap(map);
    }
}

int collisionMap::load(char *filename) {
    map = load_bitmap(filename, NULL);

    if (map == NULL) 
        return 0;

    mapWidth = map->w;
    mapHeight = map->h;
    return 1;
}


int collisionMap::load(BITMAP *newMap)
{
    if(newMap == NULL)
	return 0;
    map = newMap;
    mapWidth = map->w;
    mapHeight = map->h;

    return 1;
}
/*
 * checks to see if the postion(x,y) is a safe, safe being white, black being bad
 * return 1 if white 0 if black
 */
int collisionMap::checkcollisions(int x, int y)
{
	if(map == NULL)
	{
		printf("Error, no map loaded");
		return 0;
	}
	
	if(x > mapWidth || x < 0 || y > mapHeight || y < 0)
	{
	    return 0;
	}

	int color = getpixel(map, x,y);

	if(color == 0)
	{
	    return 1;
	}
	
	return 0;
}
