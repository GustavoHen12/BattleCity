#include "gameObject.h"

void setPosition(GameObject_t *objeto, int x, int y){
    objeto->x = x;
    objeto->y = y;
    printf("[%d %d] \n", x, y);
}

GameObject_t initGameObject(int x, int y, int dx, int dy, int type){
    GameObject_t obj;
    //seta tipo
    obj.type = type;

    //seta posicão inicial
    setPosition(&obj, x, y);
    
    //velocidade inicial
    obj.dx = dx;
    obj.dy = dy;
    obj.direction = UP;

    return obj;
}

void move(GameObject_t *object, int direction){
    //TODO: posição += velocidade
    int newPositionX, newPositionY;
    newPositionX = object->x;
    newPositionY = object->y; 
    switch (direction)
    {
    case UP:
        if(object->dy > 0)
            object->dy *= -1;
        newPositionY = object->y + object->dy;
        object->direction = UP;
        break;
    case DOWN:
        if(object->dy < 0)
            object->dy *= -1;
        newPositionY = object->y + object->dy;
        object->direction = DOWN;
        break;
    case LEFT:
        if(object->dx > 0)
            object->dx *= -1;
        newPositionX = object->x + object->dx;
        object->direction = LEFT;
        break;
    case RIGHT:
        if(object->dx < 0)
            object->dx *= -1;
        newPositionX = object->x + object->dx;
        object->direction = RIGHT;
        break;
    default:
        break;
    }
    printf(" %d " ,object->type);
    setPosition(object, newPositionX, newPositionY);
}