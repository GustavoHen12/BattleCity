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