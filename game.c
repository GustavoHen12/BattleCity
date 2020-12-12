#include"game.h"
// ------------ Testes e funções auxiliares ------------

void testMalloc(GameObject_t *obj, char *msg){
    if(!obj){
        fprintf(stderr, "[ERRO INTERNO]: Não foi possível alocar espaço para %s\n", msg);
        exit(1);
    }
}

void printObject(GameObject_t *obj){
    printf("x: %d, y: %d, dx: %d, dy: %d, type: %d, heigh: %d, widht: %d, life: %d \n",
    obj->x, obj->y, obj->dx, obj->dy, obj->type, obj->height, obj->widht, obj->life);
}

void getInitialPosition(int *x, int *y, int type, int position){
    if(type == TANK){
        *x = 30;
        *y = 40;
    }else{
        if(position == 0){
            *x = 10;
            *y = 5;
        }
        if(position == 1){
            *x = 150;
            *y = 5;
        }
        if(position == 2){
            *x = 400;
            *y = 5;
        }
    }

}
// -----------

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

// ------------ Funções jogo genéricas ------------

void InitGame(){
    //INICIA TANK
    game.tank = initGameObject(0, 0, 2, 2, TANK, TANK_H, TANK_W);
    testMalloc(&(game.tank), "tank");

    //Aloca espaco para os inimigos e testa
    game.enemies = malloc(sizeof(GameObject_t)*ENEMIES_QUANT);
    testMalloc(game.enemies, "inimigos");
    //Inicia a quantidade de inimigos em jogo para 0
    game.enemiesQuant = ENEMIES_QUANT;
    //configura posições iniciais
    for(int i = 0; i < ENEMIES_QUANT; i++){
        game.enemies[i] = initGameObject(3, 3, 1, 1, ENEMIES, ENEMIES_H, ENEMIES_W);
        game.enemies[i].life = 0;
    }

    //Aloca espaço para tiros
    game.shotsQuant = 5;
    game.shots = malloc(sizeof(GameObject_t)*game.shotsQuant);
    testMalloc(game.shots, "tiros");
    //Inicia os tiros que pertence ao inimigo
    for(int i = 0; i < game.shotsQuant; i++){
        game.shots[i] = initGameObject(0, 0, 3, 3, ENEMIE_SHOT, SHOT_H, SHOT_W);
        game.shots[i].life = 0;
    }
    //Inicia os tiros que pertencem ao TANK
    game.shots[3].type = SHOT;
    // for(int i = game.enemiesQuant; i < game.shotsQuant; i++){
    //     game.shots[i] = initGameObject(0, 0, 2, 2, SHOT, SHOT_H, SHOT_W);
    //     game.shots[i].life = 0;
    // }
    //Inicia mapa do Jogo
    initMap();

    srand(time(NULL));
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
* A partir de um "obj", retorna 1 se ele colidiu com algum tiro do tipo "type"
*/
int colisionWithShot(GameObject_t *obj, GameObject_t *shots, int shotsQuant, int type){
    for(int i = 0; i < shotsQuant ; i++){
        GameObject_t *shot = &shots[i];
        if((shot->type == type) && isAlive(shot)){
            if(colision(obj->x, obj->y, obj->height, obj->widht, 
                    shot->x, shot->y, shot->height, shot->widht)){
                softKill(shot);
                return 1;
            }            
        }
    }
    return 0;
}

/*
* Esta função recebe um objeto e uma direção, e atualiza a
* direção e a velocidade do objeto
*/
void updateDirection(GameObject_t *object, int newDirection){
    switch (newDirection)
    {
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

/*
* verifica se a posição para qual o objeto pretende ir já esta ocupada
*/
int positionEnable(int posX, int posY, int height, int widht){
    //verifica se esta dentro do campo
    if(posX < 0)
        return 0;
    if(posX+widht > BATTLE_FIELD_W)
        return 0;
    if(posY < 0)
        return 0;
    if(posY + height > BATTLE_FIELD_H)
        return 0;

    //verifica se esta dentro de alguma parede
    for(int i = 0; i < game.mapQuant; i++){
        Wall_t wall = game.map[i];
        if(colision(posX, posY, height, widht, wall.x, wall.y, wall.height, wall.width)){
            for(int j = 0; j < wall.quantBlock; j++){
                GameObject_t block;
                block = wall.blocks[j];
                if(colision(posX, posY, height, widht, block.x, block.y, 8, 22)){
                    return 0;   
                }
            }
        }
    }

    return 1;
}

int colisionWithTank(GameObject_t *obj, Game_t *game, int x, int y, int type, int index){
    for(int i = 0; i < game->enemiesQuant; i++){
        GameObject_t *enemie = &game->enemies[i];
        if(isAlive(enemie) && i != index){
            if(colision(x, y, obj->height, obj->widht, 
                enemie->x, enemie->y, enemie->height, enemie->widht))
                return 1;
        }
    }

    // if(type != TANK){
    //     GameObject_t *tank = &game->tank;
    //     if(colision(x, y, obj->height, obj->widht, 
    //         tank->x, tank->y, tank->height, tank->widht))
    //         return 1;
    // }

    return 0;
}
// ------------ Tank ------------
void updateTank(int direction){
    if(direction != -1){
        int newX = game.tank.x, newY = game.tank.y;
        int dx = game.tank.dx, dy = game.tank.dy;
        switch (direction)
        {
        case UP:
            if(game.tank.dy > 0) dy *= -1;
            newY = game.tank.y + dy;
            break;
        case DOWN:
            if(game.tank.dy < 0) dy *= -1;
            newY = game.tank.y + dy;
            break;
        case LEFT:
            if(game.tank.dx > 0) dx *= -1;
            newX = game.tank.x + dx;
            break;
        case RIGHT:
            if(game.tank.dx < 0) dx *= -1;
            newX = game.tank.x + dx;
            break;
        default:
            break;
        }
        if(positionEnable(newX, newY, 28, 28) != 0){
            move(&game.tank, direction);
        }
    }
}

// ------------ Inimigos ------------

int updateEnemies(Game_t *game){
    //---- Movimenta
    int i = 0;
    while(i < game->enemiesQuant){
        GameObject_t enemie;
        enemie = game->enemies[i];
        if(isAlive(&enemie)){
            int newX = enemie.x, newY = enemie.y;
            if(enemie.direction == UP || enemie.direction == DOWN)
                newY += enemie.dy;
            else 
                newX += enemie.dx;
            //verifica se o tank pode continuar indo na mesma direção
            if((positionEnable(newX, newY, 28, 28) != 0)
                && !colisionWithTank(&enemie, game, newX, newY, ENEMIES, i)){
                move(&(game->enemies[i]), enemie.direction);
            }else{
                //caso não possa altera direção
                enemie.direction = rand() % 4;
                updateDirection(&(game->enemies[i]), enemie.direction);
            }
        }
        i++;
    }

    //---- Verifica colisão e atira
    i = 0;
    while(i < game->enemiesQuant){
        GameObject_t *enemie;
        enemie = &game->enemies[i];
        if(isAlive(enemie)){
            //---- Verifica colisão e atira
            if(colisionWithShot(enemie, game->shots, game->shotsQuant, SHOT)){
                softKill(enemie);
                return i;
            } else {
                shoot(enemie, i);
            }
        }
        i++;
    }
    return -1;
}

int sendEnemie(Game_t *game, int *cicle){
    if(*cicle >= 180){
        *cicle = 0;
        GameObject_t *enemies = game->enemies;
        for(int i = 0; i < game->enemiesQuant; i++){
            if(!isAlive(&enemies[i])){
                return 1;
            }
        }
    }
    return 0;
}

void respawn(GameObject_t *obj, int position){
    obj->life = 1;
    getInitialPosition(&obj->x, &obj->y, ENEMIES, position);
    
}

int crateEnemie(GameObject_t *enemies, int quant, int *position){
    for(int i = 0; i < quant; i++){
        if(!isAlive(&enemies[i])){
            respawn(&enemies[i], *position);
            *position = *position == 3 ? 0:*position+1;
            return 1;
        }
    }
    return 0;
}
// ------------ Mapa ------------

/*
* A partir das opções, cria parede
* "height"x"widht", configura o tamanho do parede a ser criada (em blocos NÃO EM PIXELS)
* esta parede terá como posição superior esquerdo (x, y)
*/
void initWall(int height, int width, int x, int y){
    //incrementa quantidade de paredes no mapa
    game.mapQuant++;

    //Cria parede:
    //Pega ponteiro para a parede que será criada
    Wall_t *wall = &(game.map[game.mapQuant - 1]);
    
    //converte e salva o tamanho da parede de bloco para pixels
    wall->height = BLOCK_H*height;
    wall->width = BLOCK_W*width;
    //salva a posição inicial da parede
    wall->x = x;
    wall->y = y;

    // calcula quantos blocos serão necessarios no total e salva
    int totalBlocks = height * width;    
    wall->quantBlock = totalBlocks;

    // aloca espaço para os blocos
    wall->blocks = malloc(sizeof(GameObject_t) * totalBlocks);
    testMalloc(wall->blocks, "blocos");

    // Cria os blocos da parede:
    // é importante ressaltar que no caso dos blocos o direction
    // será utilizado para representar a "direção" em que bloco esta na parede
    // já que, as linhas são intercaladas, dando o efeito de uma parede real
    int xBlock = wall->x, yBlock = wall->y, direction = 0;
    int index = 0;
    // os blocos são criados por cada linha da parede
    for(int i = 0; i < height; i++){
        xBlock = wall->x;
        for(int j = 0; j < width; j++){
            //cria um GameObject_t do tipo bloco, com posição (x, y)
            wall->blocks[index] = initGameObject(xBlock,yBlock, 0, 0, BLOCK, BLOCK_H, BLOCK_W);
            wall->blocks[index].direction = direction;
            //incrementa indice e posição x
            index++;
            xBlock+= BLOCK_W;
        }
        //incrementa posição y e verifica qual a direção da próxima linha
        yBlock+=BLOCK_H;
        direction = direction == 0 ? 1 : 0;
    }
}

/*
* As configurações do mapa são determinadas no arquivo "resources/config.txt"
* que contém a estrutura
* |Quant                             |
* |altura largura posicão_x posição_y|
* sendo Quant, a quantidade de paredes no mapa
* e cada linha seguinte corresponde a altura, largura (ambas em blocos), 
* posição eixo X e Y, sequêncialmente.
*/
void initMap(){
    //Abre arquivo com as configurações do mapa
    char filename[25] = "resources/config.txt"; 
    FILE* arq;
	arq = fopen(filename, "r");
    if(!arq){
        fprintf(stderr, "[ERRO]: Não foi possível abrir o arquivo %s.\n", filename);
        exit(1);
    }
    
    //Lê a quantidade de paredes que o mapa possuí
    int quantWalls;
    if(fscanf(arq, "%d", &quantWalls) != 1){
            fprintf(stderr, "[ERRO INTERNO]: Erro ao ler arquivo %s, não foi possível determinar a quantidade de paredes.\n", filename);
            exit(1);
    }
    //Aloca espaço para o mapa
    game.map = malloc(sizeof(Wall_t)*quantWalls);
    if(!game.map){
        fprintf(stderr, "[ERRO INTERNO]: Não foi possível alocar espaço para o mapa\n");
        exit(1);
    }
    game.mapQuant = 0;
    //Cria as paredes do mapa
    int height, width, x, y;
    for(int i = 0; i < quantWalls; i++){
        //Lê as configurações das paredes
        if(fscanf(arq, "%d %d %d %d", &height, &width, &x, &y) != 4){
            fprintf(stderr, "[ERRO INTERNO]: Erro ao ler arquivo %s, número de entradas inválido.\n", filename);
            exit(1);
        }
        //Cria as paredes
        initWall(height, width, x, y);
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

int updateMap(Game_t *game, GameObject_t *exploded){
    int explodedCount = 0;
    for(int i = 0; i < game->mapQuant; i++){
        Wall_t *wall = &game->map[i];
        for(int j = 0; j < wall->quantBlock; j++){
            GameObject_t *block;
            block = &wall->blocks[j];
            if(colisionWithShot(block, game->shots, game->shotsQuant, SHOT)
                || colisionWithShot(block, game->shots, game->shotsQuant, ENEMIE_SHOT)){
                copyGameObject(&exploded[explodedCount], block);
                explodedCount++;
                wall->quantBlock = hardKill(wall->blocks, wall->quantBlock, j);
            }
        }

        //TODO:
        //if(wall->quantBlock == 0)
        //  free(wall);
    }
    return explodedCount;
}

// ------------ Tiro ------------
int shoot(GameObject_t *shooter, int index){
    GameObject_t *shot;
    shot = &(game.shots[index]);
    // Verifica se já não existe tiro
    if(isAlive(shot))
        return 0;
    
    int positionOffsetY = shot->height/2;
    int positionOffsetX = shot->widht/2;
    int offset = 1;
    if(shooter->direction == UP)
        setPosition(shot, shooter->x+(shooter->widht/2)-positionOffsetX, shooter->y-offset-shot->height);
    if(shooter->direction == DOWN)
        setPosition(shot, shooter->x+(shooter->widht/2)-positionOffsetX, shooter->y+shooter->height+offset);
    if(shooter->direction == LEFT)
        setPosition(shot, shooter->x-shot->widht-offset, shooter->y+(shooter->height/2)-positionOffsetY);
    if(shooter->direction == RIGHT)
        setPosition(shot, shooter->x+(shooter->widht+offset), shooter->y+(shooter->height/2)-positionOffsetX);
    
    shot->direction = shooter->direction;
    shot->life = 1;
    return 1;
}

int isInBattleField(int x, int y, int heigh, int widht){
    if(x < 0)
        return 0;
    if(y < 0)
        return 0;
    if((x+heigh) > BATTLE_FIELD_W)
        return 0;
    if((y+widht) > BATTLE_FIELD_H)
        return 0;
    return 1;
}

int updateShots(Game_t *game){
    for(int i = 0; i < game->shotsQuant; i++){
        GameObject_t *shot = &game->shots[i];
        if(isAlive(shot)){
            move(shot, shot->direction);
            //Verifica se a nova posição esta dentro do campo
            if(!isInBattleField(shot->x, shot->y, shot->height, shot->widht)){
                softKill(shot);
                return i;
            }
        }
    }

    return -1;
}

