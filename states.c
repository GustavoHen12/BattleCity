#include "states.h"

// ------------ Funções auxiliares ------------
void resetCicle(int *cicle){
    if(*cicle > 180){
        *cicle = 0;
    }
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
    drawDisplay(&game->tank);
    
    //Desenha inimigos
    for(int i = 0; i < game->enemiesQuant; i++){
        if(isAlive(&(game->enemies[i]))){
            drawDisplay(&(game->enemies[i]));
        }
    }
    
    //Desenha tiro
    for(int i = 0; i < game->shotsQuant; i++){
        if(isAlive(&game->shots[i])){
            drawDisplay(&(game->shots[i]));
        }
    }

    //Desenha mapa
    for(int i = 0; i < game->mapQuant; i++){
        Wall_t *wall = &game->map[i];
        for(int j = 0; j < wall->quantBlock; j++){
            drawWall(&wall->blocks[j], wall->type);
        }
    }

    //Desenha efeitos
    fx_draw();
}

int processGame(Game_t *game, ProcessGameInfo_t *info){
    
    updateTank(game, info->input);
    //atira
    if(info->shot){
        if(shoot(&game->tank, game->shots, TANK_SHOT_INDEX))
            playSound(FX_TYPE_SHOT);
        info->shot = 0;
    } 

    int deadEnemie = updateEnemies(game); 
    if(deadEnemie >= 0){
        int x, y;
        getMiddlePosition(game->enemies, deadEnemie, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
        playSound(FX_TYPE_EXPLOSION);
    }

    GameObject_t explodedShots[10];
    int quantExplodedShots = updateShots(game, explodedShots); 
    for(int i = 0; i < quantExplodedShots; i++){
        int x, y;
        getMiddlePosition(explodedShots, i, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
    }

    if(sendEnemie(game, info->cicle)){
        int x, y;
        getInitialPosition(&x, &y, ENEMIES, info->positionNextEnemy);
        //cria efeito do nascimento
        fx_add(x+8, y+8, FX_TYPE_CREATION);
        playSound(FX_TYPE_CREATION);
    }

    if(fx_finished(FX_TYPE_CREATION)){
        crateEnemie(game->enemies, game->enemiesQuant, &info->positionNextEnemy);
    }

    GameObject_t explodedBlocks[10];
    int quantExplodedBlocks = updateMap(game, explodedBlocks);
    for(int i = 0; i < quantExplodedBlocks; i++){
        int x, y;
        getMiddlePosition(explodedBlocks, i, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
    }

    return 0;
}

// ------------ Funções de estado ------------
/*
* Exibe menu
*/
void start(){
    initDisplay();
    initMenuDisplay();

    bool done = false;
    bool redraw = true;
    bool help = false;

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
                if(event.keyboard.keycode == ALLEGRO_KEY_S){
                    state = INIT_STAGE;
                    done = true;
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_H){
                    help = true;
                }
                if(event.keyboard.keycode == ALLEGRO_KEY_M){
                    help = false;
                }

                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                    state = LEFT_GAME;
                    done = true;
                }
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
            if(help)
                drawHelp();
            else
                drawMenu();

            //Exibe na tela
            showDraw();
            redraw = false;
        }
    }

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

    ProcessGameInfo_t info = {
        .input = -1,
        .cicle = 0,
        .positionNextEnemy = 0
    };
    
    //instancia variaveis
    initGame(&game);
    initSprites();

    //laço principal
    bool done = false;
    bool redraw = true;
    
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
                info.input = readInput(key);
                //atira
                if(key[ALLEGRO_KEY_Z]){
                    info.shot = 1;
                }
                
                processGame(&game, &info);

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
            drawInfo();

            //Exibe na tela
            showDraw();
            redraw = false;
        }
        info.cicle++;
        resetCicle(&info.cicle);
    }
    closeSound();
    closeSprites();
    state = LEFT_GAME;
}

void leftGame(){
    closeDisplay();
    state = 208;
}