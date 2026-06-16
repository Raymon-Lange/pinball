#include "pinball.h"
#include <math.h>
#include <iostream>
#include <stdio.h>

#define PI 3.1415926535

// On Linux .deb installs, PINBALL_DATA_DIR is set by CMake to the share/ path.
// On Windows/macOS, ALLEGRO_RESOURCES_PATH resolves to the directory next to the binary.
// In dev builds (no define, no WIN32/APPLE), the current directory is used (existing behaviour).
static void setupDataPath()
{
#if defined(PINBALL_DATA_DIR)
    al_change_directory(PINBALL_DATA_DIR);
#elif defined(_WIN32) || defined(__APPLE__)
    ALLEGRO_PATH *res = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    if (res) {
        al_change_directory(al_path_cstr(res, ALLEGRO_NATIVE_PATH_SEP));
        al_destroy_path(res);
    }
#endif
}

void setupscreen()
{
    al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
    display = al_create_display(WIDTH, HEIGHT);
    if(!display)
        fprintf(stderr, "Failed to create display\n");
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
}

void setupGame()
{
    pinballs[0].x = 364;
    pinballs[0].y = 661;

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

    targets->create(270, 280, 1);
    targets->create(276, 298, 1);
    targets->create(282, 317, 1);

    targets->create(143, 114, 1);
    targets->create(181, 114, 1);
    targets->create(220, 114, 1);
    targets->create(257, 114, 1);
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
    ALLEGRO_KEYBOARD_STATE kstate;
    ALLEGRO_EVENT event;

    while(waitForPlayer)
    {
        while(al_get_next_event(event_queue, &event))
        {
            if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                gameover = 1;
                return;
            }
        }

        al_get_keyboard_state(&kstate);

        if(al_key_down(&kstate, ALLEGRO_KEY_ESCAPE))
        {
            gameover = 1;
            return;
        }

        if(al_key_down(&kstate, ALLEGRO_KEY_UP) || al_key_down(&kstate, ALLEGRO_KEY_SPACE))
        {
            while(true)
            {
                al_get_keyboard_state(&kstate);
                if(!al_key_down(&kstate, ALLEGRO_KEY_UP) && !al_key_down(&kstate, ALLEGRO_KEY_SPACE))
                    break;
                power -= 0.05;
                int ratio = (int)(power * -1);
                ratio = 660 - (ratio * 13);
                al_set_target_backbuffer(display);
                al_draw_bitmap(background, 0, 0, 0);
                al_draw_filled_circle((float)pinballs[0].x, (float)pinballs[0].y, 5, al_map_rgb(255, 0, 0));
                al_draw_filled_rectangle(355, (float)ratio, 380, 660, al_map_rgb(0, 0, 255));
                al_flip_display();
                al_rest(0.010);
            }
            waitForPlayer = 0;
        }
        pinballs[0].b = power;
    }
}

void ReShoot()
{
    if(pinballs[0].x > 355 && pinballs[0].y < 665 && pinballs[0].y > 660)
        Plunger();
}

void GetInput()
{
    ALLEGRO_EVENT event;
    while(al_get_next_event(event_queue, &event))
    {
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            gameover = 1;
    }

    ALLEGRO_KEYBOARD_STATE kstate;
    al_get_keyboard_state(&kstate);

    if(al_key_down(&kstate, ALLEGRO_KEY_ESCAPE))
        gameover = 1;

    if(al_key_down(&kstate, ALLEGRO_KEY_A) || al_key_down(&kstate, ALLEGRO_KEY_LEFT))
    {
        if(leftPaddle->angle > -45)
        {
            leftPaddle->angle -= 15;
            leftPaddle->isGoingUp = 1;
        }
        else
            leftPaddle->isGoingUp = 0;
    }
    else
    {
        if(leftPaddle->angle < 0)
        {
            leftPaddle->angle += 15;
            leftPaddle->isGoingUp = 0;
        }
    }

    if(al_key_down(&kstate, ALLEGRO_KEY_D) || al_key_down(&kstate, ALLEGRO_KEY_RIGHT))
    {
        if(rightPaddle->angle < 45)
        {
            rightPaddle->angle += 15;
            rightPaddle->isGoingUp = 1;
        }
        else
            rightPaddle->isGoingUp = 0;
    }
    else
    {
        if(rightPaddle->angle > 0)
        {
            rightPaddle->angle -= 15;
            rightPaddle->isGoingUp = 0;
        }
    }
}

int BummerHit(collisionMap *cMap)
{
    int number = 0;
    for(int point = 0; point < 360; point += 10)
    {
        int x = (int)(pinballs[0].x + 5 * cos((point / 180.0) * PI));
        int y = (int)(pinballs[0].y + 5 * sin((point / 180.0) * PI));
        if(cMap->checkcollisions(x, y))
            number++;
    }
    return (number > 5) ? 1 : 0;
}

int MoveBall(collisionMap *cMap)
{
    int breakout = 0;

    pinballs[0].numAng = 0;
    pinballs[0].angX   = 0;
    pinballs[0].angY   = 0;

    for(int point = 0; point < 360; point += 10)
    {
        int x = (int)(pinballs[0].x + 5 * cos((point / 180.0) * PI));
        int y = (int)(pinballs[0].y + 5 * sin((point / 180.0) * PI));
        if(cMap->checkcollisions(x, y))
        {
            pinballs[0].numAng++;
            pinballs[0].angX += cos((point / 180.0) * PI);
            pinballs[0].angY += sin((point / 180.0) * PI);
        }
    }

    if(pinballs[0].numAng > 3)
    {
        while(pinballs[0].numAng > 20)
        {
            pinballs[0].numAng = 0;
            pinballs[0].x -= 0.1 * pinballs[0].angX;
            pinballs[0].y -= 0.1 * pinballs[0].angY;
            pinballs[0].angX = 0;
            pinballs[0].angY = 0;

            for(int point = 0; point < 360; point += 10)
            {
                int x = (int)(pinballs[0].x + 5 * cos((point / 180.0) * PI));
                int y = (int)(pinballs[0].y + 5 * sin((point / 180.0) * PI));
                if(cMap->checkcollisions(x, y))
                {
                    pinballs[0].numAng++;
                    pinballs[0].angX += cos((point / 180.0) * PI);
                    pinballs[0].angY += sin((point / 180.0) * PI);
                }
            }

            breakout++;
            if(breakout > 5000)
            {
                std::cout << "I'm Stuck" << std::endl;
                pinballs[0].numAng = 0;
                breakout = 0;
                continue;
            }
        }

        while(pinballs[0].numAng != 0)
        {
            pinballs[0].numAng = 0;
            pinballs[0].x -= 0.1 * pinballs[0].angX;
            pinballs[0].y -= 0.1 * pinballs[0].angY;

            for(int point = 0; point < 360; point += 5)
            {
                int x = (int)(pinballs[0].x + 5 * cos((point / 180.0) * PI));
                int y = (int)(pinballs[0].y + 5 * sin((point / 180.0) * PI));
                if(cMap->checkcollisions(x, y))
                    pinballs[0].numAng++;
            }

            breakout++;
            if(breakout > 5000)
            {
                std::cout << "I'm stuck 2nd" << std::endl;
                pinballs[0].numAng = 0;
                breakout = 0;
                continue;
            }
        }

        pinballs[0].x -= 0.1 * pinballs[0].angX;
        pinballs[0].y -= 0.1 * pinballs[0].angY;

        pinballs[0].angN = PI / 2.0 + atan2(pinballs[0].angY, pinballs[0].angX);

        pinballs[0].mag = sqrt(pinballs[0].a * pinballs[0].a +
                               pinballs[0].b * pinballs[0].b);

        if(pinballs[0].mag > .7)
            pinballs[0].mag -= .5;

        pinballs[0].ang = atan2(pinballs[0].b, pinballs[0].a);

        if(fabs(pinballs[0].angN - pinballs[0].ang) < .7)
        {
            pinballs[0].angN -= .05;
            if(pinballs[0].angN < pinballs[0].ang)
                pinballs[0].angN += .1;
        }

        pinballs[0].ang = -(pinballs[0].ang - pinballs[0].angN) + pinballs[0].angN;
        pinballs[0].a = pinballs[0].mag *
                        cos(pinballs[0].ang) *
                        (.3 + .7 * fabs(cos(pinballs[0].angN)));
        pinballs[0].b = pinballs[0].mag *
                        sin(pinballs[0].ang) *
                        (.3 + .7 * fabs(sin(pinballs[0].angN)));
        pinballs[0].magG = sin(pinballs[0].angN);

        if(pinballs[0].angN > PI / 2.0 && pinballs[0].angN < 3 * PI / 2.0)
        {
            pinballs[0].a += pinballs[0].magG * cos(pinballs[0].angN);
            pinballs[0].b += pinballs[0].magG * sin(pinballs[0].angN);
        }

        pinballs[0].a = (int)(pinballs[0].a * 40) / 40.0;
        pinballs[0].b = (int)(pinballs[0].b * 40) / 40.0;

        return 1;
    }

    return 0;
}

void UpdateBall()
{
    pinballs[0].x += pinballs[0].a;
    pinballs[0].y += pinballs[0].b;

    if(pinballs[0].y > HEIGHT + 5)
    {
        setupGame();
        return;
    }

    if(!MoveBall(map))
    {
        if(sqrt(pinballs[0].a * pinballs[0].a +
                (pinballs[0].b + .1) * (pinballs[0].b + .1)) < 8)
        {
            pinballs[0].b += .1;
        }
    }
}

void CheckMovingPaddleHit()
{
    if(MoveBall(paddleMap))
    {
        pinballs[0].y -= 30;
        pinballs[0].b += -8;
    }
}

void SetAfterBummer(double xpos, double ypos, double bumperCX, double bumperCY, int power)
{
    double angle = atan2((ypos - bumperCY), (xpos - bumperCX)) * 180 / PI;
    pinballs[0].a = cos(angle * PI / 180) * power;
    pinballs[0].b = sin(angle * PI / 180) * power;
    pinballs[0].x += pinballs[0].a;
    pinballs[0].y += pinballs[0].b;
}

void CheckBummerHit()
{
    if(BummerHit(leftBummer))
        SetAfterBummer(pinballs[0].x, pinballs[0].y, 122, 213, 7);
    else if(BummerHit(rightBummer))
        SetAfterBummer(pinballs[0].x, pinballs[0].y, 210, 194, 7);
    else if(BummerHit(lowBummer))
        SetAfterBummer(pinballs[0].x, pinballs[0].y, 161, 291, 7);
    else if(BummerHit(leftTriBummer))
    {
        int y = 510 + (rand() % 25);
        SetAfterBummer(42, 499, 21, y, 8);
    }
    else if(BummerHit(rightTriBummer))
    {
        int y = 510 + (rand() % 25);
        SetAfterBummer(268, 499, 302, y, 8);
    }
}

void drawBall(ALLEGRO_BITMAP *bmp)
{
    al_set_target_bitmap(bmp);
    al_draw_filled_circle((float)pinballs[0].x, (float)pinballs[0].y, 5, al_map_rgb(255, 0, 0));
}

static float paddleAngleRad(int angle)
{
    return (float)(((int)(angle * 0.771)) / 256.0 * 2.0 * PI);
}

void drawRightPaddle(ALLEGRO_BITMAP *bmp)
{
    float angle_rad = paddleAngleRad(rightPaddle->angle);
    al_set_target_bitmap(bmp);
    al_draw_tinted_scaled_rotated_bitmap(rightPaddle->map, al_map_rgb(0, 0, 0),
                                         54, 0,
                                         rightPaddle->x, rightPaddle->y,
                                         1.0, 1.0, angle_rad, 0);
}

void drawLeftPaddle(ALLEGRO_BITMAP *bmp)
{
    float angle_rad = paddleAngleRad(leftPaddle->angle);
    al_set_target_bitmap(bmp);
    al_draw_tinted_scaled_rotated_bitmap(leftPaddle->map, al_map_rgb(0, 0, 0),
                                         0, 0,
                                         leftPaddle->x, leftPaddle->y,
                                         1.0, 1.0, angle_rad, 0);
}

void drawPaddles(ALLEGRO_BITMAP *bmp)
{
    drawRightPaddle(bmp);
    drawLeftPaddle(bmp);
}

void PlacePaddles()
{
    if(rightPaddle->isGoingUp)
    {
        al_set_target_bitmap(map->map);
        al_draw_bitmap(background, 0, 0, 0);
        al_set_target_bitmap(paddleMap->map);
        al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(255, 255, 255));
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
        al_set_target_bitmap(map->map);
        al_draw_bitmap(background, 0, 0, 0);
        al_set_target_bitmap(paddleMap->map);
        al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(255, 255, 255));
        if(rightPaddle->isGoingUp)
        {
            paddleMap->load(al_create_bitmap(WIDTH, HEIGHT));
        }
        else
        {
            drawLeftPaddle(paddleMap->map);
            drawRightPaddle(map->map);
        }
    }
    else
    {
        al_set_target_bitmap(map->map);
        al_draw_bitmap(background, 0, 0, 0);
        al_set_target_bitmap(paddleMap->map);
        al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(255, 255, 255));
        drawPaddles(map->map);
    }
}

int main(void)
{
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();

    setupscreen();
    setupGame();

    setupDataPath();

    // Use memory bitmaps so al_get_pixel works without locking
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);

    ALLEGRO_BITMAP *buffer = al_create_bitmap(WIDTH, HEIGHT);
    background  = al_load_bitmap("pinball.bmp");
    paddleBuffer = al_create_bitmap(WIDTH, HEIGHT);
    al_set_target_bitmap(paddleBuffer);
    al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(255, 255, 255));

    paddleMap = new collisionMap();
    map       = new collisionMap();
    map->load("pinball.bmp");
    paddleMap->load(paddleBuffer);

    setupPaddles();
    setupTargets();
    setupBummer();

    // Show initial board before plunger
    al_set_target_backbuffer(display);
    al_draw_bitmap(background, 0, 0, 0);
    al_draw_filled_circle((float)pinballs[0].x, (float)pinballs[0].y, 5, al_map_rgb(255, 0, 0));
    al_flip_display();

    Plunger();

    while(!gameover)
    {
        GetInput();
        PlacePaddles();
        CheckBummerHit();
        UpdateBall();
        CheckMovingPaddleHit();
        targets->checkHit((int)pinballs[0].x, (int)pinballs[0].y);

        al_set_target_bitmap(buffer);
        al_draw_bitmap(background, 0, 0, 0);
        targets->draw(buffer);
        drawBall(buffer);
        drawPaddles(buffer);

        al_set_target_backbuffer(display);
        al_draw_bitmap(buffer, 0, 0, 0);
        al_flip_display();

        ReShoot();
        al_rest(0.020);
    }

    al_destroy_bitmap(buffer);
    al_destroy_display(display);
    return 0;
}
