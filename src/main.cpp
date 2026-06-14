#include "pinball.h"
#include "math.h"
#include <iostream>
#include <stdio.h>

#define PI 3.1415926535

void setupscreen()
{
    set_color_depth(16);
    int ret = set_gfx_mode(GFX_SAFE, WIDTH, HEIGHT, 0, 0);
    if(ret != 0){
       allegro_message(allegro_error);
       return;
    }
}

void setupGame()
{
    pinballs[0].x = 364;
    pinballs[0].y = 661; 
 
    //Start the ball of really fast in the y axis, 
    pinballs[0].b    = 0.00;
    pinballs[0].a    = 0.0;
    pinballs[0].ang  = 0.0;
    pinballs[0].angN = 0.0;
    pinballs[0].mag  = 0.0;
    pinballs[0].angX = 0.0;
    pinballs[0].angY = 0.0;
    pinballs[0].magG = 0.0;
}

void setupTargets()
{
    targets = new targetHandler();

    targets->create(270,280,1);
    targets->create(276,298,1);
    targets->create(282,317,1);

    targets->create(143,114,1);
    targets->create(181,114,1);
    targets->create(220,114,1);
    targets->create(257,114,1);
}

void setupPaddles()
{
    leftPaddle = new paddle();
    leftPaddle->load("showleftpaddle.bmp");
    
    leftPaddle->x = 112;
    leftPaddle->y = 600; 

    rightPaddle = new paddle();
    rightPaddle->load("showrightpaddle.bmp");

    rightPaddle->x = 252;
    rightPaddle->y = 600;
}


void setupBummer()
{
    leftBummer = new collisionMap();
    leftBummer->load("leftBummer.bmp");

    rightBummer = new collisionMap();
    rightBummer->load("rightBummer.bmp");

    lowBummer = new collisionMap();
    lowBummer->load("lowBummer.bmp");

    leftTriBummer = new collisionMap();
    leftTriBummer->load("leftTriBummer.bmp");

    rightTriBummer = new collisionMap();
    rightTriBummer->load("rightTriBummer.bmp");

}

void Plunger()
{
   int waitForPlayer = 1;
   double power = 0;
   while(waitForPlayer)
   {
       if(key[KEY_ESC])
       {
           gameover = 1;
           return;
       }

       if(key[KEY_UP])
       {
           while(key[KEY_UP])
           {
               power = power - 0.05;
               int ratio = (int)(power * -1);
               ratio = 660 - (ratio * 13);
               rectfill(screen, 355, 660, 380, ratio, makecol(0 , 0, 255)); 
               rest(10);
           }
           waitForPlayer = 0;
       }
       pinballs[0].b = power;
    }
}


void ReShoot()
{
    if(pinballs[0].x > 355 && pinballs[0].y < 665 && pinballs[0].y > 660)
    {
        Plunger();
    }
}

void GetInput()
{
    if(key[KEY_ESC])
        gameover =1;

    if(key[KEY_A] || key[KEY_LEFT])
    {
	if(leftPaddle->angle > -45 )
        {
    	    leftPaddle->angle -= 15;
            leftPaddle->isGoingUp = 1;
        }
	else
	{
	    // paddle is already at the max and not traveling up
	    leftPaddle->isGoingUp = 0;
	} 
    }
    else
    {
        if(leftPaddle->angle < 0 )
        {
            leftPaddle->angle += 15;
            leftPaddle->isGoingUp = 0;
        }
    }
    if(key[KEY_D]  || key[KEY_RIGHT])
    {
        if(rightPaddle->angle < 45 )
        {
            rightPaddle->angle += 15;
            rightPaddle->isGoingUp = 1;
        }
	else
	{
            rightPaddle->isGoingUp = 0;
	}
    }
    else
    {
        if(rightPaddle->angle > 0 )
        {
            rightPaddle->angle -= 15;
            rightPaddle->isGoingUp = 0;
        }

    }
}

int BummerHit(collisionMap * cMap)
{
    int number = 0;
    //check if any part of the circumfrance hit the wall
    for(int point = 0; point < 360; point += 10)
    {
        int x = (int)(pinballs[0].x + 5 * cos((point/180.0)* PI));
        int y = (int)(pinballs[0].y + 5 * sin((point/180.0)* PI));
        if(cMap->checkcollisions(x,y))
        {
            number++;
        }   
    }

    if(number > 5)
    {
        return 1;
    }

    return 0;
}

int MoveBall(collisionMap * cMap)
{
    int breakout = 0;    

    //reset all the number of hits, and the angle hit
    pinballs[0].numAng = 0;
    pinballs[0].angX = 0;
    pinballs[0].angY = 0;

    //check if any part of the circumfrance hit the wall
    for(int point = 0; point < 360; point += 10)
    {
        int x = (int)(pinballs[0].x + 5 * cos((point/180.0)* PI));
	    int y = (int)(pinballs[0].y + 5 * sin((point/180.0)* PI));
	    if(cMap->checkcollisions(x,y))
	    {
	        pinballs[0].numAng++;
 	        pinballs[0].angX += cos((point/180.0)* PI);
	        pinballs[0].angY += sin((point/180.0)* PI);
	    }
    }
   
   if(pinballs[0].numAng > 3)
   {
   	// while at the ball touches more 20 of the 36 possiable points
	while(pinballs[0].numAng > 20)
	{
            pinballs[0].numAng = 0;

	    //adjust the ball movement off the wall		
	    pinballs[0].x = pinballs[0].x - 0.1 * pinballs[0].angX;
	    pinballs[0].y = pinballs[0].y - 0.1 * pinballs[0].angY;

	    // recalated to make sure the ball is still 
            // no on the wall
	    pinballs[0].angX = 0;
    	    pinballs[0].angY = 0;
    	    for(int point = 0; point < 360; point += 10)
    	    {
                int x = (int)(pinballs[0].x + 5 * cos((point/180.0)* PI));
        	int y = (int)(pinballs[0].y + 5 * sin((point/180.0)* PI));

        	if(cMap->checkcollisions(x,y))
        	{
                    pinballs[0].numAng++;
            	    pinballs[0].angX += cos((point/180.0)* PI);
            	    pinballs[0].angY += sin((point/180.0)* PI);
                }
            }
            
            breakout++;
            if(breakout > 5000)
            {
                cout << "I'm Stuck" <<endl;
                pinballs[0].numAng = 0;
                breakout = 0;
                continue;
            }
        }
       
        //now move the ball completely off the wall and double the number 
        // points to check
        while(pinballs[0].numAng !=0)
        {
            pinballs[0].numAng=0;
            pinballs[0].x = pinballs[0].x - 0.1 * pinballs[0].angX;
            pinballs[0].y = pinballs[0].y - 0.1 * pinballs[0].angY;

            for(int point = 0;point<360;point=point+5)
            {
                int x = (int)(pinballs[0].x + 5 * cos((point/180.0)* PI));
	        int y = (int)(pinballs[0].y + 5 * sin((point/180.0)* PI));

          	if(cMap->checkcollisions(x,y))
                {
                    pinballs[0].numAng++;
                }
            }

            breakout++;
            if(breakout > 5000)
            {
                cout << "I'm stuck 2nd"<<endl;
                pinballs[0].numAng=0;
                breakout = 0;
                continue;
            }

	}

	pinballs[0].x = pinballs[0].x - 0.1 * pinballs[0].angX;
        pinballs[0].y = pinballs[0].y - 0.1 * pinballs[0].angY;

        pinballs[0].angN = PI/2.0 + atan2( pinballs[0].angY, pinballs[0].angX);
        
 	pinballs[0].mag = sqrt(pinballs[0].a*pinballs[0].a + 
			       pinballs[0].b*pinballs[0].b);

	
	if(pinballs[0].mag>.7)
	{
	    pinballs[0].mag = pinballs[0].mag-.5;
	}

	pinballs[0].ang = atan2(pinballs[0].b,pinballs[0].a);

	if(fabs(pinballs[0].angN - pinballs[0].ang)<.7)
	{
            pinballs[0].angN = pinballs[0].angN - .05;
            if(pinballs[0].angN < pinballs[0].ang)
	    {
		    pinballs[0].angN = pinballs[0].angN + .1;
	    }
       }

       pinballs[0].ang = -(pinballs[0].ang - pinballs[0].angN) + pinballs[0].angN;
       pinballs[0].a = pinballs[0].mag * 
                       cos(pinballs[0].ang) * 
                       (.3 + .7 * fabs(cos(pinballs[0].angN)));

       pinballs[0].b = pinballs[0].mag * 
                       sin(pinballs[0].ang)*
                       (.3 + .7 * fabs(sin(pinballs[0].angN)));

       pinballs[0].magG = sin(pinballs[0].angN);

	
       if(pinballs[0].angN > PI/2.0 && pinballs[0].angN < 3 * PI/2.0)
       {
           pinballs[0].a = pinballs[0].a + pinballs[0].magG * cos(pinballs[0].angN);
           pinballs[0].b = pinballs[0].b + pinballs[0].magG * sin(pinballs[0].angN);
       }
        
	pinballs[0].a = (int)(pinballs[0].a*40)/40.0;
        pinballs[0].b = (int)(pinballs[0].b*40)/40.0;
        
        // ball has hit the map
        return 1;
    }

    //ball has not hit the map
    return 0;

}

void UpdateBall()
{
    //move the ball at the current pace
    pinballs[0].x += pinballs[0].a;
    pinballs[0].y += pinballs[0].b;

    //Check to see if the ball hit the bottom
    if(pinballs[0].y > HEIGHT + 5)
    {
        setupGame();
	return;
    }

    if(!MoveBall(map))  
    {
	//add gravity to the ball cause it didn't shit shit
        if(sqrt(pinballs[0].a * pinballs[0].a + 
		(pinballs[0].b+.1) * (pinballs[0].b+.1)) < 8)
	{
	    pinballs[0].b = pinballs[0].b +.1;
	}
    }
}

void CheckMovingPaddleHit()
{
    // if the ball has hit a moiving paddle jump the ball up
    if(MoveBall(paddleMap))
    {
        pinballs[0].y -= 30;
        pinballs[0].b += -8;
    }
}

void SetAfterBummer(double xpos, double ypos, double bummerCenterX, double bummerCenterY, int power)
{
    double angle = atan2((ypos - bummerCenterY), (xpos - bummerCenterX)) * 180/ PI;
    //double angle = atan2((bummerCenterX - xpos),(bummerCenterY - ypos)) * 180/ PI;
    pinballs[0].a = cos(angle * PI / 180) * power;
    pinballs[0].b = sin(angle * PI / 180) * power;
    
    pinballs[0].x += pinballs[0].a;
    pinballs[0].y += pinballs[0].b;
}


void CheckBummerHit()
{
    if(BummerHit(leftBummer))
    { 
        SetAfterBummer(pinballs[0].x, pinballs[0].y, 122, 213, 7); 
    }
    else if(BummerHit(rightBummer))
    {
        SetAfterBummer(pinballs[0].x, pinballs[0].y, 210, 194, 7);
    }
    else if(BummerHit(lowBummer))
    {
        SetAfterBummer(pinballs[0].x, pinballs[0].y, 161, 291, 7);
    }
    else if(BummerHit(leftTriBummer))
    {
       int y = 510 + (rand() % 25);
       SetAfterBummer(42 , 499, 21, y, 8);
    }
    else if(BummerHit(rightTriBummer))
    {
        int y = 510 + (rand() % 25); 
        SetAfterBummer(268 , 499, 302, y, 8);
    }
}


void drawBall(BITMAP *bmp)
{
    int x = (int)pinballs[0].x;
    int y = (int)pinballs[0].y;

    circlefill(bmp, x, y, 5, RED);
}

void drawRightPaddle(BITMAP *bmp)
{
    int rx = rightPaddle->x;
    int ry = rightPaddle->y;

    int angle = (int)(rightPaddle->angle * 0.771);

    pivot_sprite(bmp, rightPaddle->map, rx, ry,54,0, itofix(angle));

}

void drawLeftPaddle(BITMAP *bmp)
{
    int x = leftPaddle->x;
    int y = leftPaddle->y;

    int angle = (int)(leftPaddle->angle * 0.771);

    pivot_sprite(bmp, leftPaddle->map, x, y,0,0, itofix(angle));
}

void drawPaddles(BITMAP *bmp)
{
    drawRightPaddle(bmp);
    drawLeftPaddle(bmp);
}

void PlacePaddles()
{
    if(rightPaddle->isGoingUp)
    {
        blit(background, map->map, 0, 0, 0, 0, WIDTH,HEIGHT);
        rectfill(paddleMap->map, 0, 0, WIDTH, HEIGHT, makecol(255,255,255));
        if(leftPaddle->isGoingUp)
        {
            drawPaddles(paddleMap->map);
        }
        else
        {
            drawRightPaddle(paddleMap->map);
            drawLeftPaddle(map->map);
        }
    }
    else if(leftPaddle->isGoingUp)
    {
        blit(background, map->map, 0, 0, 0, 0, WIDTH,HEIGHT);
        rectfill(paddleMap->map, 0, 0, WIDTH, HEIGHT, makecol(255,255,255));
        if(rightPaddle->isGoingUp)
        {
            //shouldn't happen but put the code anyway
            paddleMap->load(create_bitmap(WIDTH, HEIGHT));
        }
        else
        {
            drawLeftPaddle(paddleMap->map);
            drawRightPaddle(map->map);
        }
    }
    else
    {
        blit(background, map->map, 0, 0, 0, 0, WIDTH,HEIGHT);
        rectfill(paddleMap->map, 0, 0, WIDTH, HEIGHT, makecol(255,255,255));
        drawPaddles(map->map);
    } 
}


int main(void)
{
   allegro_init();
   install_keyboard();
   install_timer();
   setupscreen();
   setupGame();
   setupPaddles(); 
   setupTargets();

   BITMAP *buffer;
   background = create_bitmap(WIDTH, HEIGHT);
   buffer = create_bitmap(WIDTH, HEIGHT);
   paddleBuffer = create_bitmap(WIDTH, HEIGHT);
   rectfill(paddleBuffer, 0, 0, WIDTH, HEIGHT, makecol(255,255,255));

   paddleMap = new collisionMap();
   map = new collisionMap();

   background = load_bitmap("pinball.bmp" , NULL);
   map->load("pinball.bmp");
   paddleMap->load(paddleBuffer);

   blit(background, screen, 0, 0, 0, 0, WIDTH, HEIGHT);
   drawBall(screen);

   setupBummer();

   Plunger();

   while(!gameover)
   {
       GetInput();
       PlacePaddles();   
       CheckBummerHit();       
       UpdateBall();
       CheckMovingPaddleHit();
       targets->checkHit((int)pinballs[0].x, (int)pinballs[0].y);
       
       vsync();
       acquire_screen();

       //blit(map->map, buffer, 0, 0, 0, 0, WIDTH,HEIGHT);
       blit(background, buffer, 0, 0, 0, 0, WIDTH,HEIGHT);


       targets->draw(buffer);
       drawBall(buffer);
       drawPaddles(buffer);
       blit(buffer, screen, 0, 0, 0, 0, WIDTH, HEIGHT); 
       release_screen();
       
       ReShoot();
        
       rest(20);
   }

   allegro_exit();
   return 0;
}
END_OF_MAIN()
