#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#define ENEMIES_H 28
#define ENEMIES_W 30

#define TANK_H 28
#define TANK_W 28

#define SHOT_H 9
#define SHOT_W 8

#define BATTLE_FIELD_H 425
#define BATTLE_FIELD_W 434

#define BLOCK_H 8
#define BLOCK_W 16

#define EAGLE_H 33
#define EAGLE_W 32

// #define BLOCK_H 8
// #define BLOCK_W 22

enum type_obj {TANK, ENEMIES, BLOCK, EAGLE, EAGLE_FLAG_END, SHOT, ENEMIE_SHOT};

enum directions {UP, DOWN, LEFT, RIGHT};

typedef struct{
    int x, y;
} Position_t;

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

void copyGameObject(GameObject_t *dest, GameObject_t *source);

void setPosition(GameObject_t *object, int x, int y);

GameObject_t initGameObject(int x, int y, int dx, int dy, int type, int heigh, int widht);

int isAlive(GameObject_t *obj);

void updateDirection(GameObject_t *object, int newDirection);

void kill(GameObject_t *object);

void revive(GameObject_t *object);

#endif