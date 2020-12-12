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

#define EAGLE_H
#define EAGLE_W

// #define BLOCK_H 8
// #define BLOCK_W 22

enum type_obj {TANK, ENEMIES, BLOCK, EAGLE, FLAG, SHOT, ENEMIE_SHOT};

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

#endif