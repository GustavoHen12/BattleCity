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
GameObject_t initGameObject(int x, int y, int dx, int dy, int type, int heigh, int widht);

void InitGame();

//movimenta tanque
void move(GameObject_t *tank, int direction);
void updateTank(int direction);

//movimenta jogo
void nextCicle(int *cicle);

//atira
int shoot(GameObject_t *shooter, int index);

int updateShots(Game_t *game);

//veirifa colisao


int updateEnemies(Game_t *game);

//adiciona inimigo
void createEnemies(GameObject_t *enemies);

int isAlive(GameObject_t *obj);

void initMap();

int updateMap(Game_t *game, GameObject_t *exploded);

void getMiddlePosition(GameObject_t *obj, int index, int *x, int *y);

#endif