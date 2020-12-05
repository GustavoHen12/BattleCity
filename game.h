//LOGICA DO JOGO
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"gameObject.h"

#define MAX_SHOOT_SIZE 200
#define INTERVAL_GENERATE_ENEMIES 180
#define ENEMIES_QUANT 4

typedef struct{
    GameObject_t *map;
    int mapSize;

    GameObject_t *enemies;
    int enemiesSize;

    GameObject_t *shoots;
    int shootsSize;

    GameObject_t eagle;
} Game_t;

Game_t game;
//inicia
GameObject_t initGameObject(int x, int y, int dx, int dy, int type);

void InitGame();

//movimenta tanque
void move(GameObject_t *tank, int direction);

//movimenta jogo
void nextCicle(int *cicle);

//atira
void shoot(GameObject_t *shooter);
//veirifa colisao

//adiciona inimigo
void updateEnemies();