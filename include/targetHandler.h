#include "target.h"

class targetHandler
{
private:
    int count;
    target *targets[100];

public:
    targetHandler(void);
    ~targetHandler(void);
    void add(target *trg);
    void create(int xpos, int ypos, int type);
    int size(){ return count; }
    void removeAll();
    void checkHit(int xpos, int ypos);
    void draw(BITMAP *bmp);
};
