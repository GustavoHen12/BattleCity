#include"game.h"
// ------------ Testes e funções auxiliares ------------

void testMalloc(GameObject_t *obj, char *msg){
    if(!obj){
        fprintf(stderr, "[ERRO INTERNO]: Não foi possível alocar espaço para %s\n", msg);
        exit(1);
    }
}

// ------------ Funções para manipular o GameObject_t ------------

void setPosition(GameObject_t *objeto, int x, int y){
    objeto->x = x;
    objeto->y = y;
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

// ------------ Funções jogo genéricas ------------
void InitGame(){
    //Aloca espaco para os inimigos e testa
    game.enemies = malloc(sizeof(GameObject_t)*ENEMIES_QUANT);
    testMalloc(game.enemies, "inimigos");
    //Inicia a quantidade de inimigos em jogo para 0
    game.enemiesSize = ENEMIES_QUANT;
    //configura posições iniciais
    for(int i = 0; i < ENEMIES_QUANT; i++){
        game.enemies[i] = initGameObject(30*(i + 1), 30*(i+1), 0, 0, ENEMIES);
    }

    //Aloca espaço para tiros
    game.shoots = malloc(sizeof(GameObject_t)*2);
    testMalloc(game.shoots, "tiros");
    //só pode haver 1 tiro do tank em jogo
    game.shootsSize = 1;
    //coloca o tiro fora do campo
    for(int i = 0; i < game.shootsSize; i++){
        game.shoots[i] = initGameObject(-10, -10, 2, 2, SHOT);
    }

    //Aloca espaço para paredes do mapa
    game.map = malloc(sizeof(Wall_t)*12);
    testMalloc(game.shoots, "mapa");
}

void nextCicle(int *cicle){
    // if(*cicle >= INTERVAL_GENERATE_ENEMIES){
    //     if(game.enemiesSize < ENEMIES_QUANT){
    //         game.enemiesSize++;
    //     }
    //     *cicle = 0;
    // }
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
    setPosition(object, newPositionX, newPositionY);
}

int colision(GameObject_t *objA, int heightA, int widhtA, GameObject_t *objB, int heightB, int widthB){
    int xA, yA, xB, yB;
    xA = objA->x;
    yA = objA->y;
    xB = objB->x;
    yB = objB->y;

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
* Para matar um objeto, ao invés de retirar seu espaço de memória
* ele é trocado com o último da lista e retorna o novo tamanho da lista
*/
int kill(GameObject_t *objectList, int size, int index){
    objectList[index].life = 0;
    GameObject_t temp = objectList[index];
    objectList[index] = objectList[size - 1];
    objectList[size - 1] = temp;
    return size - 1;
}
// ------------ Tank ------------

// ------------ Inimigos ------------

int updateEnemies(){
    //TODO: Movimenta
    // verifica se colidiu com tiro
    GameObject_t shot = game.shoots[game.shootsSize - 1];
    int i = 0;
    while(i < game.enemiesSize){
        GameObject_t enemie;
        enemie = game.enemies[i];
        if(colision(&enemie, 30, 32, &shot, 9, 8)){
            game.enemiesSize = kill(game.enemies, game.enemiesSize, i);
            return 1;
        }else{
            i++;
        }
    }
    return 0;
}

// ------------ Mapa ------------
void initMap(){
    Wall_t *wall = &(game.map[0]);
    
    wall->height = 80;
    wall->width = 44;
    wall->x = 10;
    wall->y = 20;

    // pega o tamanho, altura e direcao da parede
    int height = wall->height, widht = wall->width;

    // calcula quantos blocos serão necessarios
    int heighBlocks = height / BLOCK_HEIGHT;
    int widthBlocks = widht / BLOCK_WIDTH;
    int totalBlocks = heighBlocks * widthBlocks;
    
    wall->quantBlock = totalBlocks;

    printf(" %d %d %d\n", totalBlocks, heighBlocks, widthBlocks);
    // aloca espaço para os blocos
    wall->blocks = malloc(sizeof(GameObject_t) * totalBlocks);
    testMalloc(wall->blocks, "blocos");

    // cria os blocos
    int x = wall->x, y = wall->y;
    int index = 0;
    for(int i = 0; i < heighBlocks; i++){
        x = wall->x;
        for(int j = 0; j < widthBlocks; j++){
            wall->blocks[index] = initGameObject(x,y, 0, 0, BLOCK);
            index++;
            x+= BLOCK_WIDTH;
        }
        y+=BLOCK_HEIGHT;
    }
}
// ------------ Tiro ------------

void shoot(GameObject_t *shooter){
    //TODO: Arrumar posição inicial dos tiros
    //TODO: Verificar se já não existe um tiro em jogo
    //TODO: Verificar colisão com paredes
    GameObject_t *shot;
    shot = &(game.shoots[game.shootsSize - 1]);
    if(shooter->direction == UP)
        setPosition(shot, shooter->x + 10, shooter->y - 4);
    if(shooter->direction == DOWN)
        setPosition(shot, shooter->x + 10, shooter->y + 28);
    if(shooter->direction == LEFT)
        setPosition(shot, shooter->x + 1, shooter->y + 17);
    if(shooter->direction == RIGHT)
        setPosition(shot, shooter->x + 30, shooter->y + 15);
    
    shot->direction = shooter->direction;
}

void updateShot(){
    GameObject_t *shot;
    shot = &(game.shoots[game.shootsSize - 1]);
    move(shot, shot->direction);
}