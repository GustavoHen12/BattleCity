//LOGICA DO JOGO
#include<stdio.h>

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
    int life
} GameObject_t;

//inicia

//veirifa colisao

//adiciona inimigo

//movimenta tanque
setPosition();
//movimenta jogo