#ifndef __GAME_BASE__
#define __GAME_BASE__

#include"gameObject.h"

void getInitialPosition(int *x, int *y, int type, int position);

void copyGameObject(GameObject_t *dest, GameObject_t *source);

void setPosition(GameObject_t *objeto, int x, int y);

GameObject_t initGameObject(int x, int y, int dx, int dy, int type, int heigh, int widht);

int isAlive(GameObject_t *obj);

void getMiddlePosition(GameObject_t *obj, int index, int *x, int *y);

void respawn(GameObject_t *obj, int position);

void softKill(GameObject_t *object);

int hardKill(GameObject_t *objectList, int size, int index);

int colision(int xA, int yA, int heightA, int widhtA, int xB, int yB, int heightB, int widthB);

void updateDirection(GameObject_t *object, int newDirection);

void move(GameObject_t *object, int direction);
#endif