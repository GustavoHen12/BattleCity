#include"game.h"
// ------------ Funções para trabalalhar com os dados do jogo ------------
void increaseScore (GameData_t *data){
    data->score += data->pointsPerKill;
}

void decraseLife (GameData_t *data){
    data->life--;
}

void decreaseEnemiesRemainig(GameData_t *data){
    data->enemiesRemaining--;
}

void decrasePointsPerKill(GameData_t *data){
    data->pointsPerKill -= 3;
}

// ------------ Testes e funções básicas ------------

void testMalloc(GameObject_t *obj, char *msg){
    if(!obj){
        fprintf(stderr, "[ERRO INTERNO]: Não foi possível alocar espaço para %s\n", msg);
        exit(1);
    }
}

// ------------ Funções jogo genéricas ------------

void initGame(Game_t *game){
    //INICIA TANK
    int x, y;
    getInitialPosition(&x, &y, TANK, 0);
    game->tank = initGameObject(x, y, 2, 2, TANK, TANK_H, TANK_W);
    game->tank.life = TANK_LIFES;
    game->eagle = initGameObject(BATTLE_FIELD_W/2 - 15, 392, 0, 0, EAGLE, TANK_H, TANK_W);
    
    //Aloca espaco para os inimigos e testa
    game->enemies = malloc(sizeof(GameObject_t)*ENEMIES_QUANT);
    testMalloc(game->enemies, "inimigos");
    //Inicia a quantidade de inimigos em jogo para 0
    game->enemiesQuant = ENEMIES_QUANT;
    //configura posições iniciais
    for(int i = 0; i < ENEMIES_QUANT; i++){
        game->enemies[i] = initGameObject(3, 3, 1, 1, ENEMIES, ENEMIES_H, ENEMIES_W);
        game->enemies[i].life = 0;
    }

    //Aloca espaço para tiros
    game->shotsQuant = SHOTS_QUANT;
    game->shots = malloc(sizeof(GameObject_t)*game->shotsQuant);
    testMalloc(game->shots, "tiros");
    //Inicia os tiros que pertence ao inimigo
    for(int i = 0; i < game->shotsQuant; i++){
        game->shots[i] = initGameObject(0, 0, 3, 3, ENEMIE_SHOT, SHOT_H, SHOT_W);
        game->shots[i].life = 0;
    }
    //Inicia o tiro que pertence ao TANK
    game->shots[TANK_SHOT_INDEX].type = SHOT;
 
    //Inicia mapa do Jogo
    initMap(game);

    srand(time(NULL));
}

/*
* verifica se a posição para qual o objeto pretende ir já esta ocupada
*/
int positionEnable(Game_t *game, int posX, int posY, int height, int widht){
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
    for(int i = 0; i < game->mapQuant; i++){
        Wall_t wall = game->map[i];
        //A parede deve ser diferente de arbusto, pois este pode ser atravessado
        if(wall.type != BUSH){
            //se colide com a parede verifica se colide com algum bloco
            if(colision(posX, posY, height, widht, wall.x, wall.y, wall.height, wall.width)){
                for(int j = 0; j < wall.quantBlock; j++){
                    GameObject_t block;
                    block = wall.blocks[j];
                    //se colide com o bloco retorna 0
                    if(colision(posX, posY, height, widht, block.x, block.y, block.height, block.widht)){
                        return 0;   
                    }
                }
            }
        }
    }
    // Caso a posição esteja livre retorna 1
    return 1;
}
/*
* Verifica se o elemento de posição x,y e tamanho heighxWidht
* está dentro do campo de jogo
*/
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


/*
* A partir de um "obj", retorna 1 se ele colidiu com algum tiro do tipo "type"
*/
int colisionWithShot(GameObject_t *obj, GameObject_t *shots, int shotsQuant, int type){
    for(int i = 0; i < shotsQuant ; i++){
        GameObject_t *shot = &shots[i];
        //percorre a lista de tiro e compara com rodos os que estão vivo
        // e são do tipo "type"
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
* Confere se o "obj", colide com algum tanque na posição x, y
* Caso seja do tipo inimigo, o index é passado para evidar que ele compare com si mesmo
*/
int colisionWithTank(GameObject_t *obj, Game_t *game, int x, int y, int type, int index){
    //percorre a lista de tanques inimigos
    //comparando com todos os que estão vivo e não estão na posição index
    for(int i = 0; i < game->enemiesQuant; i++){
        GameObject_t *enemy = &game->enemies[i];
        if(isAlive(enemy) && i != index){
            if(colision(x, y, obj->height, obj->widht, 
                enemy->x, enemy->y, enemy->height, enemy->widht))
                return 1;
        }
    }
    //se o "obj" for o tanque ele não pode ser
    //comparado com ele mesmo
    if(type != TANK){
        GameObject_t *tank = &game->tank;
        if(colision(x, y, obj->height, obj->widht, 
            tank->x, tank->y, tank->height, tank->widht))
            return 1;
    }

    return 0;
}

// ------------ Tank ------------
int updateTank(Game_t *game, int direction){
    if(direction != -1){
       //Calcula qual a direção que o usuário pretende ir
        int newX = game->tank.x, newY = game->tank.y;
        int dx = game->tank.dx, dy = game->tank.dy;
        switch (direction){
            case UP:
                if(game->tank.dy > 0) dy *= -1;
                newY = game->tank.y + dy;
                break;
            case DOWN:
                if(game->tank.dy < 0) dy *= -1;
                newY = game->tank.y + dy;
                break;
            case LEFT:
                if(game->tank.dx > 0) dx *= -1;
                newX = game->tank.x + dx;
                break;
            case RIGHT:
                if(game->tank.dx < 0) dx *= -1;
                newX = game->tank.x + dx;
                break;
            default:
                break;
        }
        //Verifica se a posição não é parede ou um tanque
        if(positionEnable(game, newX, newY, 28, 28)
            && !colisionWithTank(&game->tank, game, newX, newY, TANK, ENEMIES_QUANT)){
            move(&game->tank, direction);
        }
    }

    //---- Colisão com tiro
    //Se o tanque for atingido volta pra posição inicial e diminui 1 de vida
    if(colisionWithShot(&game->tank, game->shots, game->shotsQuant, ENEMIE_SHOT)){
        softKill(&game->tank);
        respawn(&game->tank, 0);
        return 1;
    }

    return 0;
}

// ------------ Inimigos ------------

int updateEnemies(Game_t *game){
    //---- Movimenta
    int i = 0;
    while(i < game->enemiesQuant){
        GameObject_t enemie;
        enemie = game->enemies[i];
        //se estiver vivo
        if(isAlive(&enemie)){
            //Calcula a nova posição
            int newX = enemie.x, newY = enemie.y;
            if(enemie.direction == UP || enemie.direction == DOWN)
                newY += enemie.dy;
            else 
                newX += enemie.dx;
            //verifica se o tank pode continuar indo na mesma direção
            if((positionEnable(game, newX, newY, 28, 28) != 0)
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
            //Se não foi atingido tenta atirar
            if(colisionWithShot(enemie, game->shots, game->shotsQuant, SHOT)){
                softKill(enemie);
                //se for atingido retorna o inidice do tanque atingido
                return i;
            } else {
                shoot(enemie, game->shots, i);
            }
        }
        i++;
    }
    return -1;
}

int sendEnemy(Game_t *game, int cicle, int enemiesRemaining){
    //Verifica se é necessário gerar um novo inimigo
    if(cicle >= INTERVAL_GENERATE_ENEMIES && enemiesRemaining > 0){
        GameObject_t *enemies = game->enemies;
        //percorre a lista verificando se a algum morto que passará por um respawn
        for(int i = 0; i < game->enemiesQuant; i++){
            if(!isAlive(&enemies[i])){
                return 1;
            }
        }
    }
    return 0;
}

int crateEnemie(GameObject_t *enemies, int quant, int *position){
    //Percorre a lista e seleciona o que estiver morto para renascer na base "position"
    for(int i = 0; i < quant; i++){
        if(!isAlive(&enemies[i])){
            respawn(&enemies[i], *position);
            updateDirection(&enemies[i], DOWN);
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
void initWall(Wall_t *wall, int height, int width, int x, int y, int type){
    // Estas variaveis são iniciadas, porque o tamanho do bloco varia conforme
    // o tipo da parede
    int blockHeigh = BLOCK_H, blockWidht = BLOCK_W;
    if(type != BRICK){
        blockHeigh *= 2;
    }

    //Cria parede:    
    //salva o tipo da parede
    wall->type = type;

    //converte e salva o tamanho da parede de bloco para pixels
    wall->height = blockHeigh*height;
    wall->width = blockWidht*width;
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
            wall->blocks[index] = initGameObject(xBlock,yBlock, 0, 0, BLOCK, blockHeigh, blockWidht);
            wall->blocks[index].direction = direction;
            //incrementa indice e posição x
            index++;
            xBlock+= blockWidht;
        }
        //incrementa posição y e verifica qual a direção da próxima linha
        yBlock+=blockHeigh;
        direction = direction == 0 ? 1 : 0;
    }
}

/*
* As configurações do mapa são determinadas no arquivo "resources/config.txt"
* que contém a estrutura
* |Quant                                   |
* |altura largura posicão_x posição_y, tipo|
* sendo Quant, a quantidade de paredes no mapa
* e cada linha seguinte corresponde, respectivamente a, altura, largura (ambas em unidade de blocos), 
* posição eixo X, posição eixo Y e tipo da parede (tijolo, pedra ou arbusto).
*/
void initMap(Game_t *game){
    //Abre arquivo com as configurações do mapa
    FILE* arq = openFile("resources/config.txt", "r");
    
    //Lê a quantidade de paredes que o mapa possuí
    int quantWalls;
    if(fscanf(arq, "%d", &quantWalls) != 1){
            fprintf(stderr, "[ERRO INTERNO]: Erro ao ler o mapa, não foi possível determinar a quantidade de paredes.\n");
            exit(1);
    }
    //Aloca espaço para o mapa
    game->map = malloc(sizeof(Wall_t)*quantWalls);
    if(!game->map){
        fprintf(stderr, "[ERRO INTERNO]: Não foi possível alocar espaço para o mapa\n");
        exit(1);
    }
    game->mapQuant = 0;
    //Cria as paredes do mapa
    int height, width, x, y, type;
    for(int i = 0; i < quantWalls; i++){
        //Lê as configurações das paredes
        if(fscanf(arq, "%d %d %d %d %d", &height, &width, &x, &y, &type) != 5){
            fprintf(stderr, "[ERRO INTERNO]: Erro ao ler o mapa, número de entradas inválido.\n");
            exit(1);
        }
        //Cria a parede
        initWall(&(game->map[game->mapQuant]), height, width, x, y, type);
        //incrementa quantidade de paredes no mapa
        game->mapQuant++;
    }
}

int updateMap(Game_t *game, GameObject_t *exploded){
    int explodedCount = 0;
    for(int i = 0; i < game->mapQuant; i++){
        Wall_t *wall = &game->map[i];
        //para cada parede (que não seja arbusto)
        if((wall->type == BRICK) || (wall->type == STONE)){
            //percorre cada bloco
            for(int j = 0; j < wall->quantBlock; j++){
                GameObject_t *block;
                block = &wall->blocks[j];
                //se ocorreu a colisão com algum tiro (do tanque ou do inimigo)
                if(colisionWithShot(block, game->shots, game->shotsQuant, SHOT)
                    || colisionWithShot(block, game->shots, game->shotsQuant, ENEMIE_SHOT)){
                    //adiciona na lista e aumenta o tamanho dela
                    copyGameObject(&exploded[explodedCount], block);
                    explodedCount++;
                    //se for tijolo, é retirado da parede
                    if(wall->type==BRICK){
                        wall->quantBlock = hardKill(wall->blocks, wall->quantBlock, j);
                    }
                }
            }
        }
    }

    //retorna quantidade de blocos removidos
    return explodedCount;
}

// ------------ Tiro ------------
int shoot(GameObject_t *shooter, GameObject_t *shots, int index){
    GameObject_t *shot;
    shot = &(shots[index]);
    // Verifica se já não existe tiro
    if(isAlive(shot)){
        return 0;
    }
    
    // Se não existe, a partir da direção do atirador
    // calcula a posição do tiro
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
    // A direção do tiro é a mesma do atirador no momento que realizou o tiro 
    shot->direction = shooter->direction;
    // Ativa o tiro
    shot->life = 1;
    return 1;
}

int updateShots(Game_t *game, GameObject_t *exploded){
    int quantExploded = 0;
    //Verifica e movimenta o tiro
    for(int i = 0; i < game->shotsQuant; i++){
        GameObject_t *shot = &game->shots[i];
        if(isAlive(shot)){
            //se estiver vivo movimenta ao tiro
            move(shot, shot->direction);
            //Verifica se a nova posição esta dentro do campo
            if(!isInBattleField(shot->x, shot->y, shot->height, shot->widht)){
                //se não estiver remove o tiro e adiciona na lista dos "explodidos"
                softKill(shot);
                copyGameObject(&exploded[quantExploded], shot);
                quantExploded++;
            }
        }
    }

    //Verifica se um tiro do tanque colidiu com o do inimigo
    if(colisionWithShot(&game->shots[TANK_SHOT_INDEX], game->shots, game->shotsQuant, ENEMIE_SHOT)){
        softKill(&game->shots[TANK_SHOT_INDEX]);
    }
    
    return quantExploded;
}

int verifyGameOver(Game_t *game, GameData_t *data){
    // ---- Verifica se o Brasão foi atingido
    if(colisionWithShot(&game->eagle, game->shots, game->shotsQuant, SHOT)){
        game->eagle.type = EAGLE_FLAG_END;
        return 1;
    }

    if(colisionWithShot(&game->eagle, game->shots, game->shotsQuant, ENEMIE_SHOT)){
        game->eagle.type = EAGLE_FLAG_END;
        return 1;
    }

    // ---- Verifica se o Tank esta vivo
    if(data->life == 0){
        return 1;
    }

    // ---- Verifica se ainda resta inimigos
    if(data->enemiesRemaining == 0){
        for(int i = 0; i < game->enemiesQuant; i++){
            if(isAlive(&game->enemies[i]))
                return 0;
        }   
        return 1;
    }

    return 0;
}