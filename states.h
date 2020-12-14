#ifndef __STATES__
#define __STATES__

#include <stdio.h>

#include "game.h"
#include "display.h"

enum{ START, INIT_STAGE, PLAY, GAME_OVER, LEFT_GAME } state;

enum {MENU_SCREEN, HELP_SCREEN, INIT_SCREEN, GAME_OVER_SCREEN} screenType;

/*
* Configurações iniciais de tela, exibe menu e tela de ajuda
*/
void start();

/*
* Tela de preparação pro jogo
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

/*
* Exibe Score
*/
void gameOver();

#endif