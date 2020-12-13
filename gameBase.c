#include"gameBase.h"

void getInitialPosition(int *x, int *y, int type, int position){
    int margin = 7;
    if(type == TANK){
        *x = 155;
        *y = 395;
    }else{
        if(position == 0){
            *x = margin;
            *y = margin;
        }
        if(position == 1){
            *x = (BATTLE_FIELD_W / 2) - (ENEMIES_W/2);
            *y = margin;
        }
        if(position == 2){
            *x = BATTLE_FIELD_W - margin - ENEMIES_W;
            *y = margin;
        }
    }

}

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
    obj.direction = UP;

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

void respawn(GameObject_t *obj, int position){
    obj->life = 1;
    getInitialPosition(&obj->x, &obj->y, ENEMIES, position);   
}

/*
* Não altera o tamanho da lista, apenas subtrai 1 da vida
*/
void softKill(GameObject_t *object){
    object->life -= 1;
}

/*
* Para matar um objeto, ao invés de liberar seu espaço de memória
* ele é trocado com o último da lista e retorna o novo tamanho da lista
*/
int hardKill(GameObject_t *objectList, int size, int index){
    softKill(&objectList[index]);
    GameObject_t temp = objectList[index];
    objectList[index] = objectList[size - 1];
    objectList[size - 1] = temp;
    return size - 1;
}


int colision(int xA, int yA, int heightA, int widhtA, int xB, int yB, int heightB, int widthB){
    if(xA > xB + widthB) 
        return 0;
    if(xA + widhtA < xB)
        return 0;
    if(yA > yB + heightB)
        return 0;
    if(yA + heightA < yB)
        return 0;

    return 1;
}

/*
* Esta função recebe um objeto e uma direção, e atualiza a
* direção e a velocidade do objeto
*/
void updateDirection(GameObject_t *object, int newDirection){
    switch (newDirection) {
        case UP:
            if(object->dy > 0)
                object->dy *= -1;
            object->direction = UP;
            break;
        case DOWN:
            if(object->dy < 0)
                object->dy *= -1;
            object->direction = DOWN;
            break;
        case LEFT:
            if(object->dx > 0)
                object->dx *= -1;
            object->direction = LEFT;
            break;
        case RIGHT:
            if(object->dx < 0)
                object->dx *= -1;
            object->direction = RIGHT;
            break;
        default:
            break;
    }
}

/*
* Faz a movimentação de um "object" para uma direction
* É importante ressaltar que esta função atualiza a direção
* alterando a velocidade e, obviemente, a posição do objeto
*/
void move(GameObject_t *object, int direction){
    //Verifica a posição atual
    int newPositionX, newPositionY;
    newPositionX = object->x;
    newPositionY = object->y;
    //Altera a direção e a velocidade do objeto
    updateDirection(object, direction); 
    //Calcula a nova posição
    if(direction == UP || direction == DOWN)
        newPositionY = object->y + object->dy;
    else 
        newPositionX = object->x + object->dx;
    //Atualiza posição do objeto
    setPosition(object, newPositionX, newPositionY);
}