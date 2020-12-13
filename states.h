#ifndef __STATES__
#define __STATES__

#include <stdio.h>

#include "game.h"
#include "display.h"

enum{ START, INIT_STAGE, PLAY, GAME_OVER, LEFT_GAME } state;

/*
* Configurações iniciais de tela, exibe menu
*/
void start();

/*
* Configurações do jogo
*/
void init_stage();

/*
* Laço principal do jogo
*/
void play();

/*
* Finaliza o processo do jogo
*/
void leftGame();

#endif