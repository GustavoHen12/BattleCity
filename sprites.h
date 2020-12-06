#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
//TODO: Mover novamente para display.h
//define tamanho dos sprites(px)
#define TANK_H 30
#define TANK_W 30

#define ENEMIES_H 
#define ENEMIES_w

#define BLOCK_H 8
#define BLOCK_W 22

#define EAGLE_H
#define EAGLE_W

#define FLAG_H
#define FLAG_W

#define SHOT_H 9
#define SHOT_W 8

enum {SPRITE_TANK, SPRITE_ENEMIES, SPRITE_BLOCK, SPRITE_EAGLE, SPRITE_FLAG, SPRITE_SHOT} sprite_type;

enum {SPRITE_UP, SPRITE_DOWN, SPRITE_LEFT, SPRITE_RIGHT} sprite_directions;

#define EXPLOSION_FRAMES 3

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* tank[4];
    ALLEGRO_BITMAP* enemies[4];

    ALLEGRO_BITMAP* shots[4];

    ALLEGRO_BITMAP* explosion[4];

    ALLEGRO_BITMAP* block[2];
    ALLEGRO_BITMAP* eagle;
    ALLEGRO_BITMAP* flag;
} SPRITES;

SPRITES sprites;
