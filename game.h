#ifndef __GAME__
#define __GAME__

//LOGICA DO JOGO
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include"gameObject.h"

#define MAX_SHOOT_SIZE 200
#define INTERVAL_GENERATE_ENEMIES 180
#define ENEMIES_QUANT 1

#define BLOCK_WIDTH 16
#define BLOCK_HEIGHT 8

typedef struct{
    GameObject_t *blocks;
    int x, y, height, width, quantBlock;
} Wall_t;

typedef struct{
    GameObject_t tank;
    Wall_t *map;
    int mapQuant;

    GameObject_t *enemies;
    int enemiesQuant;

    GameObject_t *shots;
    int shotsQuant;

    GameObject_t eagle;
} Game_t;

Game_t game;

//inicia
GameObject_t initGameObject(int x, int y, int dx, int dy, int type);

void InitGame();

//movimenta tanque
void move(GameObject_t *tank, int direction);
void updateTank(int direction);

//movimenta jogo
void nextCicle(int *cicle);

//atira
void shoot(GameObject_t *shooter);
void updateShot();

//veirifa colisao

//adiciona inimigo
int updateEnemies();

void initMap();

#endif