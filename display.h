#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "sprites.h"

//inicia as variáveis globais que serão necessárias 
ALLEGRO_TIMER* timer;
ALLEGRO_EVENT_QUEUE* queue;
ALLEGRO_DISPLAY* screen;
ALLEGRO_FONT* font;
ALLEGRO_BITMAP* buffer;

//configurações da tela
#define DISPLAY_SCALE 1
#define BUFFER_WIDHT 450
#define BUFFER_HEIGHT 650
#define DISPLAY_HEIGH (BUFFER_HEIGHT * DISPLAY_SCALE)
#define DISPLAY_WIDHT (BUFFER_WIDHT * DISPLAY_SCALE)


typedef ALLEGRO_EVENT TimerEvent_t;

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
void drawDisplay(int x, int y, ALLEGRO_BITMAP *sprite);

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
