//LOGICA DO JOGO
#include<stdio.h>
#include<stdlib.h>

//todo: não sei se isso vai funcionar
enum {TANK, ENEMIES, BLOCK, EAGLE, FLAG} type_obj;

typedef struct {
    //posicao
    int x, y;
    //velocidade
    int dx, dy;
    //tipo: tanque, inimigo, bloco...
    int type;
    //quantidade de vidas
    int life;
} GameObject_t;

//inicia
GameObject_t initGameObject(int x, int y, int type);

//movimenta tanque
void setPosition(GameObject_t *objeto, int x, int y);

//movimenta jogo

//veirifa colisao

//adiciona inimigo