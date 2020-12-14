#ifndef __DISPLAY__
#define __DISPLAY__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include"gameObject.h"

#define EXPLOSION_FRAMES 3
#define CREATION_FRAMES 6



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


/*
* Esta funcao inicia um display de DISPLAY_HEIGHxDISPLAY_WIDHT
* e faz as configurações iniciais e inicializa as variaveis globais
* Também realiza a inicialização do spritesheet e dos sons
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
* Exibe as imagens que foram desenhadas na tela
*/
void showDraw();

/*
* Destroi os objetos necessários e fecha display
*/
void closeDisplay();

/*
* Desenha um bloco de uma paredo do tipo typeWall
*/
void drawWall(GameObject_t *obj, int typeWall);

// ------------ Info ------------
typedef struct {
    ALLEGRO_BITMAP* tankIcon;
    ALLEGRO_BITMAP* enemyIcon;
} InfoIcons_t;

InfoIcons_t icons;

/*
* Desenha as informações da partida, como pontuação, score...
*/
void drawInfo(int life, int score, int enemiesRemaining);

/*
* inicia os icones do tanque e do inimigo que são definidos
* na struct icons
*/
void initIcons();

// Destroi os icones do tipo bitmap em struct icons
void closeIcons();

// ------------ Sprites ------------

enum sprite_type {SPRITE_BRICK, SPRITE_STONE, SPRITE_BUSH};

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

/*
* Inicia e carrega os sprites do jogo na struct sprites
*/
void initSprites();

/*
* Fecha os sprites definidos
*/
void closeSprites();

/*
* desenha um sprite na tela, nas posições (x, y)
*/
void drawSprite(GameObject_t *obj);


// ------------ Menu (tela inicial e de ajuda) ------------
typedef struct {
    ALLEGRO_BITMAP* logo;

    char *help;
    char *startGame;
    char *finish;

} MenuDisplayInfo_t;

MenuDisplayInfo_t menuDisplay;
/*
* Carrega a logo e alguns textos definidos na struct menu
*/
void initMenuDisplay();

/*
* Desenha Menu principal
*/
void drawMenu();

/*
* Desenha tela de ajuda
*/
void drawHelp();

/*
* Fecha bitmaps e libera memoria alocada em "menuDisplay"
*/
void closeMenu();

// ---------- Game Over ------------
/*
* Desenha "Game over" sobre a tela
*/
void drawGameOver();
/*
* Exibe a tela com o top 5 e o último score
*/
void drawScore(int *topScore, int lastScore);

// ---------- Som ------------
typedef struct {
    ALLEGRO_SAMPLE *explosion;
    ALLEGRO_SAMPLE *creation;
    ALLEGRO_SAMPLE *shot;
} Sounds_t;

Sounds_t sounds;
/*
* A partir do tipo do efeito (ver definições), toca o efeito desejado
*/
void playSound(int type);

void closeSound();

// ------------ Efeitos ------------
/*
* As funções e a lógica para implementação dos efeitos foram 
* trazidas e adaptadas do tutoria "Allegro Vivace"
* VER README
*/

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


#endif