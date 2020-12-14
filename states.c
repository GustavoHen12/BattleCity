#include "states.h"

// ------------ Funções auxiliares ------------
int endCicle(int *cicle){
    if(*cicle > 180){
        *cicle = 0;
        return 1;
    }
    return 0;
}

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

int processGame(Game_t *game, ProcessGameInfo_t *process, GameData_t *data){
    if(updateTank(game, process->input)){
        decraseLife(data);
    }
    //atira
    if(process->shot){
        if(shoot(&game->tank, game->shots, TANK_SHOT_INDEX))
            playSound(FX_TYPE_SHOT);
        process->shot = 0;
    } 

    int deadEnemie = updateEnemies(game); 
    if(deadEnemie >= 0){
        int x, y;
        getMiddlePosition(game->enemies, deadEnemie, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
        playSound(FX_TYPE_EXPLOSION);
        increaseScore(data);
    }

    GameObject_t explodedShots[10];
    int quantExplodedShots = updateShots(game, explodedShots); 
    for(int i = 0; i < quantExplodedShots; i++){
        int x, y;
        getMiddlePosition(explodedShots, i, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
    }

    if(sendEnemy(game, process->cicle, data->enemiesRemaining)){
        int x, y;
        getInitialPosition(&x, &y, ENEMIES, process->positionNextEnemy);
        //cria efeito do nascimento
        fx_add(x+8, y+8, FX_TYPE_CREATION);
        playSound(FX_TYPE_CREATION);
    }

    if(fx_finished(FX_TYPE_CREATION)){
        crateEnemie(game->enemies, game->enemiesQuant, &process->positionNextEnemy);
        decreaseEnemiesRemainig(data);
    }

    GameObject_t explodedBlocks[10];
    int quantExplodedBlocks = updateMap(game, explodedBlocks);
    for(int i = 0; i < quantExplodedBlocks; i++){
        int x, y;
        getMiddlePosition(explodedBlocks, i, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
    }

    return verifyGameOver(game, data);
}

int drawScreen(int screenType){
    // Para a tela GAME OVER
    int topScore[5];
    int lastScore = 0;

    // Para a tela INIT
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

    return screenState;
}
// ------------ Funções de estado ------------
/*
* Exibe menu
*/
void start(){
    initDisplay();
    initMenuDisplay();

    state = drawScreen(MENU_SCREEN);

    closeMenu();
}

/*
* Tela com o nome da fase
*/
void init_stage(){
    state = PLAY;
}

/*
* Jogo em si
*/
void play(){
    Game_t game;

    ProcessGameInfo_t process = {
        .input = -1,
        .cicle = 0,
        .positionNextEnemy = 0
    };

    GameData_t data = {
        .score = 0,
        .pointsPerKill = 60,
        .enemiesRemaining = 18,
        .life = TANK_LIFES
    };
    
    //instancia variaveis
    initGame(&game);
    initSprites();
    initIcons();

    //laço principal
    bool done = false;
    bool redraw = true;
    int gameOver = 0;

    TimerEvent_t event;
    
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    
    fx_init();
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
                if(!gameOver){
                    gameOver = processGame(&game, &process, &data);
                }
                
                fx_update();
                
                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;

                if(key[ALLEGRO_KEY_ESCAPE]) done = true;

                redraw = true;
                break;
            
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;
            
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
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
            
            //Desenha margem
            drawInfo(data.life, data.score, data.enemiesRemaining);

            if(gameOver){
                drawGameOver();
            }

            //Exibe na tela
            showDraw();
            redraw = false;
        }
        process.cicle++;
        if(endCicle(&process.cicle)){
            decrasePointsPerKill(&data);
            if(gameOver){
                done = true;
            }
        }
    }
    saveScore(data.score);
    closeSprites();
    closeIcons();
    state = GAME_OVER;
}

void leftGame(){
    closeDisplay();
    closeSound();
}

void gameOver(){
    state = drawScreen(GAME_OVER);
}