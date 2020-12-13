#include"gameObject.h"

void copyGameObject(GameObject_t *dest, GameObject_t *source){
    dest->x = source->x;
    dest->y = source->y;
    dest->dx = source->dx;
    dest->dy = source->dy;
    dest->type = source->type;
    dest->life = source->life;
    dest->height = source->height;
    dest->widht = source->widht;
}

void setPosition(GameObject_t *objeto, int x, int y){
    objeto->x = x;
    objeto->y = y;
}

GameObject_t initGameObject(int x, int y, int dx, int dy, int type, int heigh, int widht){
    GameObject_t obj;
    //seta tipo
    obj.type = type;

    //seta posicão inicial
    setPosition(&obj, x, y);
    
    //velocidade inicial
    obj.dx = dx;
    obj.dy = dy;

    //tamanho
    obj.height = heigh;
    obj.widht = widht;

    //Configurações automaticas
    obj.life = 1;
    obj.direction = DOWN;

    return obj;
}

int isAlive(GameObject_t *obj){
    return obj->life > 0;
}

void getMiddlePosition(GameObject_t *obj, int index, int *x, int *y){
    GameObject_t *object = &obj[index];
    *x = object->x + (object->widht/2);
    *y = object->y + (object->height/2);
}