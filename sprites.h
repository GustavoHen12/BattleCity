#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
//TODO: Mover novamente para display.h

enum {SPRITE_TANK, SPRITE_ENEMIES, SPRITE_BLOCK, SPRITE_EAGLE, SPRITE_FLAG, SPRITE_SHOT} sprite_type;

enum {SPRITE_UP, SPRITE_DOWN, SPRITE_LEFT, SPRITE_RIGHT} sprite_directions;

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* tank[4];
    ALLEGRO_BITMAP* enemies[4];

    ALLEGRO_BITMAP* shots[4];

    ALLEGRO_BITMAP* block;
    ALLEGRO_BITMAP* eagle;
    ALLEGRO_BITMAP* flag;
} SPRITES;

SPRITES sprites;
