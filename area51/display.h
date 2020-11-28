#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* disp;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* buffer;

#define DISPLAY_SCALE 1
#define BUFFER_WIDHT 450
#define BUFFER_HEIGHT 650
#define DISPLAY_HEIGH (BUFFER_HEIGHT * DISPLAY_SCALE)
#define DISPLAY_WIDHT (BUFFER_WIDHT * DISPLAY_SCALE)

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* ship;

    ALLEGRO_BITMAP* shipEnemy;
    ALLEGRO_BITMAP* shipEnemy2;

} SPRITES;
SPRITES sprites;


int initDisplay();

void startFPS();

void drawDisplay(int x, int y, ALLEGRO_BITMAP *sprite);

void beforeDraw();

void showDraw();

void closeDisplay();

void sprites_init();

ALLEGRO_BITMAP *loadSprite(char *filename);