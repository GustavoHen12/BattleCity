//define tamanho dos sprites(px)
#define TANK_H 30
#define TANK_W 30

#define ENEMIES_H 
#define ENEMIES_w

#define BLOCK_H
#define BLOCK_W

#define EAGLE_H
#define EAGLE_W

#define FLAG_H
#define FLAG_W

#define SHOT_H 9
#define SHOT_W 8

//todo: não sei se isso vai funcionar
enum {TANK, ENEMIES, BLOCK, EAGLE, FLAG, SHOT} type_obj;

enum {UP, DOWN, LEFT, RIGHT} directions;

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