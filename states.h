#include <stdio.h>

#include "game.h"
#include "display.h"

enum{ START, INIT_STAGE, PLAY, GAME_OVER, LEFT_GAME } state;

/*
*Laço principal do jogo
*/
void play();

/*
* Configurações iniciais, exibe menu
*/
void start();