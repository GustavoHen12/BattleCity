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