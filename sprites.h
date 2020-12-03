#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

//define tamanho dos sprites(px)
#define TANK_H 20 //(1.5, 25)
#define TANK_W 19
#define ENEMIES_H
#define ENEMIES_w
#define BLOCK_H
#define BLOCK_W
#define EAGLE_H
#define EAGLE_W
#define FLAG_H
#define FLAG_W

enum {SPRITE_UP, SPRITE_DOWN, SPRITE_LEFT, SPRITE_RIGHT} sprite_directions;

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* tank[4];
    ALLEGRO_BITMAP* enemies[4];

    ALLEGRO_BITMAP* block;
    ALLEGRO_BITMAP* eagle;
    ALLEGRO_BITMAP* flag;
} SPRITES;

SPRITES sprites;
