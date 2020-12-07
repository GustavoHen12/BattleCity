#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include <stdio.h> //TODO: VERIFICAR SE DA PRA TIRAR ISSO

enum type_obj {TANK, ENEMIES, BLOCK, EAGLE, FLAG, SHOT};

enum directions {UP, DOWN, LEFT, RIGHT};

typedef struct {
    //posicao
    int x, y;
    //velocidade
    int dx, dy;
    //tipo: tanque, inimigo, bloco...
    int type;
    //quantidade de vidas
    int life;
    //direção
    int direction;
    //tamanho
    int height, widht;
} GameObject_t;


void setPosition(GameObject_t *objeto, int x, int y);

GameObject_t initGameObject(int x, int y, int dx, int dy, int type);

void move(GameObject_t *object, int direction);

#endif