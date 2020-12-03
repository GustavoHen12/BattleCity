#include"game.h"

void setPosition(GameObject_t *objeto, int x, int y){
    objeto->x = x;
    objeto->y = y;
}

GameObject_t initGameObject(int x, int y, int type){
    GameObject_t obj;
    //seta tipo
    obj.type = type;

    //seta posicão inicial
    setPosition(&obj, x, y);

    obj.direction = UP;

    return obj;
}

void InitGame(){
    game.enemies = malloc(sizeof(GameObject_t)*ENEMIES_QUANT);
    if(!game.enemies){
        fprintf(stderr, "Não foi possível alocar espaço para os inimigos \n");
        exit(1);
    }
    game.enemiesSize = 0;
    for(int i = 0; i < ENEMIES_QUANT; i++){
        game.enemies[i] = initGameObject(30*(i + 1), 30*(i+1), ENEMIES);
    }
}

void moveTank(GameObject_t *tank, int direction){
    switch (direction)
    {
    case UP:
        setPosition(tank, tank->x, tank->y - 1);
        tank->direction = UP;
        break;
    case DOWN:
        setPosition(tank, tank->x, tank->y + 1);
        tank->direction = DOWN;
        break;
    case LEFT:
        setPosition(tank, tank->x-1, tank->y);
        tank->direction = LEFT;
        break;
    case RIGHT:
        setPosition(tank, tank->x+1, tank->y);
        tank->direction = RIGHT;
        break;
    default:
        break;
    }
}

void nextCicle(int *cicle){
    if(*cicle >= INTERVAL_GENERATE_ENEMIES){
        if(game.enemiesSize < ENEMIES_QUANT){
            game.enemiesSize++;
        }
        *cicle = 0;
    }
}