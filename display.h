#ifndef __DISPLAY__
#define __DISPLAY__

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include"gameObject.h"

// ------------ Sprites ------------
//define tamanho dos sprites(px)

#define EXPLOSION_FRAMES 3
#define CREATION_FRAMES 6

enum sprite_type {SPRITE_BRICK, SPRITE_STONE, SPRITE_BUSH};

enum sprite_directions {SPRITE_UP, SPRITE_DOWN, SPRITE_LEFT, SPRITE_RIGHT};

typedef struct SPRITES
{
    ALLEGRO_BITMAP* tank[4];
    ALLEGRO_BITMAP* enemies[4];

    ALLEGRO_BITMAP* shots[4];

    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
    ALLEGRO_BITMAP* creation[CREATION_FRAMES];

    ALLEGRO_BITMAP* block[2];
    ALLEGRO_BITMAP* stone;
    ALLEGRO_BITMAP* bush;

    ALLEGRO_BITMAP* eagle;
    ALLEGRO_BITMAP* flag;
} Sprites_t;

Sprites_t sprites;

typedef struct {
    ALLEGRO_SAMPLE *explosion;
    ALLEGRO_SAMPLE *creation;
    ALLEGRO_SAMPLE *shot;
} Sounds_t;

Sounds_t sounds;
// ------------ Menu ------------
typedef struct {
    ALLEGRO_BITMAP* logo;

    char *help;
    char *startGame;
    char *finish;

} MenuDisplayInfo_t;

MenuDisplayInfo_t menuDisplay;

// ------------ Variaveis globais e config do display ------------
//inicia as variáveis globais que serão necessárias
//são utilizada apenas no arquivo display.c
ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* screen;
ALLEGRO_FONT* font;

ALLEGRO_BITMAP* _sheet;

//configurações da tela
#define DISPLAY_HEIGH 425
#define DISPLAY_WIDHT 550

#define KEY_SEEN     1
#define KEY_RELEASED 2

typedef ALLEGRO_EVENT TimerEvent_t;

// ------------ Efeitos ------------
#define FX_TYPE_EXPLOSION 0
#define FX_TYPE_CREATION 1
#define FX_TYPE_SHOT 2

typedef struct FX
{
    int x, y;
    int frame;
    bool spark;
    bool used;
    int type;
} FX;

#define FX_N 128
FX fx[FX_N];

void fx_init();
void fx_draw();
void fx_update();
void fx_add(int x, int y, int type);
int fx_finished(int type);

void playSound(int type);
/*
* Esta funcao inicia um display de DISPLAY_HEIGHxDISPLAY_WIDHT
* e faz as configurações iniciais e inicializa as variaveis globais
*/
int initDisplay();

/*
* inicia a contagem do event time
*/
void startFPS();

/*
* Limpa a tela, altera o alvo para o buffer
* desenha os objetos fixos
*/
void beforeDraw();

/*
* desenha um sprite na tela, nas posições (x, y)
*/
void drawDisplay(GameObject_t *obj);

/*
* desenha um sprite na tela, nas posições (x, y)
*/
void drawWall(GameObject_t *obj, int typeWall);

void drawInfo();

void initSprites();

void initMenuDisplay();

void drawMenu();

void drawHelp();

void closeSound();

void closeSprites();

void closeMenu();

/*
* Exibe as imagens que foram desenhadas na tela
*/
void showDraw();

/*
* Destroi os objetos necessários e fecha display
*/
void closeDisplay();

/*
* Carrega os sprites definidos em "sprites.h"
*/
void spritesInit();

void testaSprite( GameObject_t *obj, GameObject_t *obj2);
#endif