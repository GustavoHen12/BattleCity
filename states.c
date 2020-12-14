#include "states.h"

// ------------ Funções auxiliares ------------
//Verifica se o ciclo deve ser finalizado (cada ciclo dura 3seg)
//retorna 1 se sim
int endCicle(int *cicle){
    if(*cicle > 180){
        *cicle = 0;
        return 1;
    }
    return 0;
}

//Lê a direção que o usuário que ir
int readInput(unsigned char key[ALLEGRO_KEY_MAX]){
    if(key[ALLEGRO_KEY_LEFT])
        return LEFT;
    if(key[ALLEGRO_KEY_RIGHT])
        return RIGHT;
    if(key[ALLEGRO_KEY_UP])
        return UP;
    if(key[ALLEGRO_KEY_DOWN])
        return DOWN;
    return -1;
}

//Desenha tela principal do jogo
void drawGame(Game_t *game){    
    // Desenha Tank
    drawSprite(&game->tank);
    
    //Desenha inimigos
    for(int i = 0; i < game->enemiesQuant; i++){
        if(isAlive(&(game->enemies[i]))){
            drawSprite(&(game->enemies[i]));
        }
    }
    
    //Desenha tiro
    for(int i = 0; i < game->shotsQuant; i++){
        if(isAlive(&game->shots[i])){
            drawSprite(&(game->shots[i]));
        }
    }

    //Desenha mapa
    for(int i = 0; i < game->mapQuant; i++){
        Wall_t *wall = &game->map[i];
        for(int j = 0; j < wall->quantBlock; j++){
            drawWall(&wall->blocks[j], wall->type);
        }
    }

    // Desenha Tank
    drawSprite(&game->eagle);

    //Desenha efeitos
    fx_draw();
}

//Processa um ciclo do jogo
int processGame(Game_t *game, ProcessGameInfo_t *process, GameData_t *data){
    //Processa tanque
    if(updateTank(game, process->input, process->ghostMode)){
        decraseLife(data);
    }
    //Se o usuário atirou
    if(process->shot){
        //tenta atirar
        if(shoot(&game->tank, game->shots, TANK_SHOT_INDEX))
            playSound(FX_TYPE_SHOT);
        process->shot = 0;
    } 

    //Processa inimigos
    int deadEnemie = updateEnemies(game); 
    if(deadEnemie >= 0){
        //Se algum morreu:
        int x, y;
        //adiciona efeito
        getMiddlePosition(game->enemies, deadEnemie, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
        //Toca som de explosão
        playSound(FX_TYPE_EXPLOSION);
        //Aumenta pontuação
        increaseScore(data);
    }

    //Processa tiros
    GameObject_t explodedShots[10];
    int quantExplodedShots = updateShots(game, explodedShots); 
    //Para cada tiro que saiu da tela, adiciona efeito de explosão
    for(int i = 0; i < quantExplodedShots; i++){
        int x, y;
        getMiddlePosition(explodedShots, i, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
    }

    //Verifica se é necessário novo inimigo
    if(sendEnemy(game, process->cicle, data->enemiesRemaining)){
        //Pega posição inicial
        int x, y;
        getInitialPosition(&x, &y, ENEMIES, process->positionNextEnemy);
        //cria efeito do nascimento
        fx_add(x+8, y+8, FX_TYPE_CREATION);
        playSound(FX_TYPE_CREATION);
    }
    //Após finalizar o efeito do nascimento
    if(fx_finished(FX_TYPE_CREATION)){
        //adiciona o inimigo na tela
        crateEnemie(game->enemies, game->enemiesQuant, &process->positionNextEnemy);
        //diminui a quantidade de inimigos restantes
        decreaseEnemiesRemainig(data);
    }

    //Verifica quais blocos de paredes foram atingidos por tiros
    GameObject_t explodedBlocks[10];
    int quantExplodedBlocks = updateMap(game, explodedBlocks);
    for(int i = 0; i < quantExplodedBlocks; i++){
        int x, y;
        getMiddlePosition(explodedBlocks, i, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
    }

    //Verifica fim de jogo
    return verifyGameOver(game, data);
}

//Desenha tela dependendo do "screenType"
int drawScreen(int screenType){
    // Para a tela GAME OVER
    int topScore[5];
    int lastScore = 0;

    bool done = false;
    bool redraw = true;
    
    int type = screenType;
    int screenState = state;

    TimerEvent_t event;
    startFPS();
    while(1){
        al_wait_for_event(queue, &event);
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                // Lê a entrada conforme o tipo da tela
                if(type == MENU_SCREEN || type == GAME_OVER_SCREEN){
                    if(event.keyboard.keycode == ALLEGRO_KEY_S){
                        screenState = INIT_STAGE;
                        done = true;
                    }
                }
                if(type == MENU_SCREEN){
                    if(event.keyboard.keycode == ALLEGRO_KEY_H){
                        type = HELP_SCREEN;
                    }
                }
                if(type == HELP_SCREEN){
                    if(event.keyboard.keycode == ALLEGRO_KEY_M){
                            type = MENU_SCREEN;
                    }
                }

                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                    screenState = LEFT_GAME;
                    done = true;
                }
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                screenState = LEFT_GAME;
                done = true;
                break;
        }

        if(done)
            break;
        
        if(redraw && al_is_event_queue_empty(queue)){
            //Configurações iniciais
            beforeDraw();
            //Desenha conforme o tipo da tela
            switch (type){
                case MENU_SCREEN:
                    drawMenu();
                    break;
                case HELP_SCREEN:
                    drawHelp();
                    break;
                case GAME_OVER_SCREEN:
                    getTopScore(topScore, &lastScore);
                    drawScore(topScore, lastScore);
                    break;
            default:
                break;
            }
            //Exibe na tela
            showDraw();
            redraw = false;
        }
    }
    //retorna o próximo estado
    return screenState;
}

// ------------ Funções de estado ------------
/*
* Exibe menu
*/
void start(){
    //Inicia as configurações iniciais da tela
    initDisplay();
    //Inicia os elementos referentes ao menu
    initMenuDisplay();
    //Exibe menu, e recebe o valor do próximo estado
    state = drawScreen(MENU_SCREEN);
    //Fechar elementos do menu
    closeMenu();
}

/*
* Tela com o nome da fase
*/
void init_stage(){
    //drawScreen(INIT_SCREEN);
    //playSound();
    state = PLAY;
}

/*
* Jogo em si
*/
void play(){
    //Variaveis do jogo
    Game_t game;

    ProcessGameInfo_t process = {
        .input = -1,
        .cicle = 0,
        .positionNextEnemy = 0,
        .ghostMode = 0
    };

    GameData_t data = {
        .score = 0,
        .pointsPerKill = 60,
        .enemiesRemaining = 18,
        .life = TANK_LIFES
    };
    
    //inicia jogo e elementos da tela
    initGame(&game);
    initSprites();
    initIcons();

    //variáveis necessárias
    bool done = false;
    bool redraw = true;
    int gameOver = 0;

    TimerEvent_t event;
    
    //Vetor com tecla pressinada
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    
    fx_init();
    //Laço principal
    startFPS();
    while(1){
        al_wait_for_event(queue, &event);
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                //movimenta tank
                process.input = readInput(key);
                //atira
                if(key[ALLEGRO_KEY_Z]){
                    process.shot = 1;
                }
                //ghosMode
                if(key[ALLEGRO_KEY_G]){
                    process.ghostMode = 1;
                }
                //Caso não tenha ocorrido o fim do jogo
                if(!gameOver){
                    //Processa o jogo
                    //recebe 1 se o ocorreu um fim de jogo
                    gameOver = processGame(&game, &process, &data);
                }
                //Atualiza os efeitos
                fx_update();
                
                //Limpa teclas
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                if(key[ALLEGRO_KEY_ESCAPE]) done = true;

                redraw = true;
                break;
            
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;

                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                    state = LEFT_GAME;
                    done = true;
                }
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;
            
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                state = LEFT_GAME;
                done = true;
                break;
        }

        if(done)
            break;
        
        if(redraw && al_is_event_queue_empty(queue)){
            //Configurações iniciais
            beforeDraw();

            //Desenha objetos do jogo
            drawGame(&game);
            
            //Desenha margem e informações da partida
            drawInfo(data.life, data.score, data.enemiesRemaining);

            //Se ocorreu um fim de jogo
            if(gameOver){
                drawGameOver();
            }

            //Exibe na tela
            showDraw();
            redraw = false;
        }
        //incrementa ciclo
        process.cicle++;
        if(endCicle(&process.cicle)){
            //A cada ciclo:
            //diminui peso dos pontos
            decrasePointsPerKill(&data);
            //se ocorreu um fim de jogo, ao final do ciclo
            //o loop é encerrado
            if(gameOver){
                done = true;
            }
        }
    }
    //Salva score
    saveScore(data.score);
    //Fecha sprites
    closeSprites();
    closeIcons();
    //se o loop não foi encerrado pelo usuário vai pra GAME_OVER
    if(state != LEFT_GAME)
        state = GAME_OVER;
}

/*
* Exibe a tela de score
*/
void gameOver(){
    state = drawScreen(GAME_OVER);
}

/*
* Fecha a tela do jogo
*/
void leftGame(){
    closeDisplay();
    closeSound();
}