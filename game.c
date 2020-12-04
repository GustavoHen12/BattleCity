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

void testMalloc(GameObject_t *obj, char *msg){
    if(!obj){
        fprintf(stderr, "[ERRO INTERNO]: Não foi possível alocar espaço para %s\n", msg);
        exit(1);
    }
}

void InitGame(){
    //Aloca espaco para os inimigos e testa
    game.enemies = malloc(sizeof(GameObject_t)*ENEMIES_QUANT);
    testMalloc(game.enemies, "inimigos");
    //Inicia a quantidade de inimigos em jogo para 0
    game.enemiesSize = 0;
    //configura posições iniciais
    for(int i = 0; i < ENEMIES_QUANT; i++){
        game.enemies[i] = initGameObject(30*(i + 1), 30*(i+1), ENEMIES);
    }

    //Aloca espaço para tiros
    game.shoots = malloc(sizeof(GameObject_t)*2);
    testMalloc(game.shoots, "tiros");
    //só pode haver 1 tiro do tank em jogo
    game.shootsSize = 1;
    //coloca o tiro fora do campo
    for(int i = 0; i < game.shootsSize; i++){
        game.shoots[i] = initGameObject(-10, -10, SHOT);
    }
}

void move(GameObject_t *object, int direction){
    switch (direction)
    {
    case UP:
        setPosition(object, object->x, object->y - 1);
        object->direction = UP;
        break;
    case DOWN:
        setPosition(object, object->x, object->y + 1);
        object->direction = DOWN;
        break;
    case LEFT:
        setPosition(object, object->x-1, object->y);
        object->direction = LEFT;
        break;
    case RIGHT:
        setPosition(object, object->x+1, object->y);
        object->direction = RIGHT;
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

    GameObject_t *shot;
    shot = &(game.shoots[game.shootsSize - 1]);
    move(shot, shot->direction);
}

void shoot(GameObject_t *shooter){
    
    GameObject_t *shot;
    shot = &(game.shoots[game.shootsSize - 1]);
    if(shooter->direction == UP)
        setPosition(shot, shooter->x + 15, shooter->y + 1);
    if(shooter->direction == DOWN)
        setPosition(shot, shooter->x + 15, shooter->y + 30);
    if(shooter->direction == LEFT)
        setPosition(shot, shooter->x + 1, shooter->y + 17);
    if(shooter->direction == RIGHT)
        setPosition(shot, shooter->x + 30, shooter->y + 15);
    
    shot->direction = shooter->direction;
}