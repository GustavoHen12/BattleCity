//LOGICA DO JOGO
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//todo: não sei se isso vai funcionar
enum {TANK, ENEMIES, BLOCK, EAGLE, FLAG} type_obj;

enum {UP, DOWN, LEFT, RIGHT, SHOOT} directions;

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
} GameObject_t;

//inicia
GameObject_t initGameObject(int x, int y, int type);

//movimenta tanque
void moveTank(GameObject_t *tank, int direction);

void setPosition(GameObject_t *objeto, int x, int y);

//movimenta jogo

//veirifa colisao

//adiciona inimigo