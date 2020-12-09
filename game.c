#include"game.h"
// ------------ Testes e funções auxiliares ------------

void testMalloc(GameObject_t *obj, char *msg){
    if(!obj){
        fprintf(stderr, "[ERRO INTERNO]: Não foi possível alocar espaço para %s\n", msg);
        exit(1);
    }
}
// -----------

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
    game.tank = initGameObject(0, 0, 1, 1, TANK);
    testMalloc(&(game.tank), "tank");

    //Aloca espaco para os inimigos e testa
    game.enemies = malloc(sizeof(GameObject_t)*ENEMIES_QUANT);
    testMalloc(game.enemies, "inimigos");
    //Inicia a quantidade de inimigos em jogo para 0
    game.enemiesQuant = ENEMIES_QUANT;
    //configura posições iniciais
    for(int i = 0; i < ENEMIES_QUANT; i++){
        game.enemies[i] = initGameObject(3, 3, 2, 2, ENEMIES);
        game.enemies[i].direction = DOWN;
    }

    //Aloca espaço para tiros
    game.shots = malloc(sizeof(GameObject_t)*2);
    testMalloc(game.shots, "tiros");
    //só pode haver 1 tiro do tank em jogo
    game.shotsQuant = 1;
    //coloca o tiro fora do campo
    for(int i = 0; i < game.shotsQuant; i++){
        game.shots[i] = initGameObject(-10, -10, 2, 2, SHOT);
    }

    //Inicia mapa do Jogo
    initMap();

    srand(time(NULL));
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
* Para matar um objeto, ao invés de liberar seu espaço de memória
* ele é trocado com o último da lista e retorna o novo tamanho da lista
*/
int kill(GameObject_t *objectList, int size, int index){
    objectList[index].life = 0;
    GameObject_t temp = objectList[index];
    objectList[index] = objectList[size - 1];
    objectList[size - 1] = temp;
    return size - 1;
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
    // #define DISPLAY_HEIGH 500
    // #define DISPLAY_WIDHT 650   
    //verifica se esta dentro do campo
    if(posX < 0)
        return 0;
    if(posX+widht > 650)
        return 0;
    if(posY < 0)
        return 0;
    if(posY + height > 500)
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

int updateEnemies(){
    //---- Movimenta
    int i = 0;
    while(i < game.enemiesQuant){
        GameObject_t enemie;
        enemie = game.enemies[i];
        int newX = enemie.x, newY = enemie.y;
        if(enemie.direction == UP || enemie.direction == DOWN)
            newY += enemie.dy;
        else 
            newX += enemie.dx;
        //verifica se o tank pode continuar indo na mesma direção
        if(positionEnable(newX, newY, 28, 28) != 0){
            move(&(game.enemies[i]), enemie.direction);
        }else{
            //caso não possa altera direção
            enemie.direction = rand() % 4;
            updateDirection(&(game.enemies[i]), enemie.direction);
        }
        i++;
    }

    //---- Verifica colisão
    GameObject_t shot = game.shots[game.shotsQuant - 1];
    i = 0;
    while(i < game.enemiesQuant){
        GameObject_t enemie;
        enemie = game.enemies[i];
        if(colision(enemie.x, enemie.y, 30, 32, shot.x, shot.y, 9, 8)){
            game.enemiesQuant = kill(game.enemies, game.enemiesQuant, i);
            return 1;
        }else{
            i++;
        }
    }

    //---- Atira
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
    wall->height = BLOCK_HEIGHT*height;
    wall->width = BLOCK_WIDTH * width;
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
            wall->blocks[index] = initGameObject(xBlock,yBlock, 0, 0, BLOCK);
            wall->blocks[index].direction = direction;
            //incrementa indice e posição x
            index++;
            xBlock+= BLOCK_WIDTH;
        }
        //incrementa posição y e verifica qual a direção da próxima linha
        yBlock+=BLOCK_HEIGHT;
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
        //Lễ as configurações das paredes
        if(fscanf(arq, "%d %d %d %d", &height, &width, &x, &y) != 4){
            fprintf(stderr, "[ERRO INTERNO]: Erro ao ler arquivo %s, número de entradas inválido.\n", filename);
            exit(1);
        }
        //Cria as paredes
        initWall(height, width, x, y);
    }
}

// ------------ Tiro ------------
void shoot(GameObject_t *shooter){
    //TODO: Arrumar posição inicial dos tiros
    //TODO: Verificar se já não existe um tiro em jogo
    //TODO: Verificar colisão com paredes
    GameObject_t *shot;
    shot = &(game.shots[game.shotsQuant - 1]);
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
    shot = &(game.shots[game.shotsQuant - 1]);
    move(shot, shot->direction);
}