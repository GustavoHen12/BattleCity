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

void processGame(Game_t *game, int input, int cicle){
    updateTank(input);
    //atira
    if(key[ALLEGRO_KEY_Z]){
        if(shoot(&game->tank, TANK_SHOT_INDEX))
            playSound(FX_TYPE_SHOT);
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

    if(sendEnemie(game, &cicle)){
        int x, y;
        getInitialPosition(&x, &y, ENEMIES, positionNextEnemie);
        //cria efeito do nascimento
        fx_add(x+8, y+8, FX_TYPE_CREATION);
        playSound(FX_TYPE_CREATION);
    }

    if(fx_finished(FX_TYPE_CREATION)){
        crateEnemie(game->enemies, game->enemiesQuant, &positionNextEnemie);
    }

    GameObject_t explodedBlocks[10];
    int quantExplodedBlocks = updateMap(game, explodedBlocks);
    for(int i = 0; i < quantExplodedBlocks; i++){
        int x, y;
        getMiddlePosition(explodedBlocks, i, &x, &y);
        fx_add(x, y, FX_TYPE_EXPLOSION);
    }
}

// ------------ Funções de estado ------------

void start(){
    state = INIT_STAGE;
}

void init_stage(){
    state = PLAY;
}

void play(){
    //instancia variaveis
    InitGame();
    initDisplay();
   
    int input = -1;
    int cicle = 0;
    int positionNextEnemie = 0;
    
    //Position_t initialPositions[4];

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
                input = readInput(key);
                updateTank(input);
                //atira
                if(key[ALLEGRO_KEY_Z]){
                    if(shoot(&game.tank, TANK_SHOT_INDEX))
                        playSound(FX_TYPE_SHOT);
                }

                int deadEnemie = updateEnemies(&game); 
                if(deadEnemie >= 0){
                    int x, y;
                    getMiddlePosition(game.enemies, deadEnemie, &x, &y);
                    fx_add(x, y, FX_TYPE_EXPLOSION);
                    playSound(FX_TYPE_EXPLOSION);
                }

                GameObject_t explodedShots[10];
                int quantExplodedShots = updateShots(&game, explodedShots); 
                for(int i = 0; i < quantExplodedShots; i++){
                    int x, y;
                    getMiddlePosition(explodedShots, i, &x, &y);
                    fx_add(x, y, FX_TYPE_EXPLOSION);
                }

                if(sendEnemie(&game, &cicle)){
                    int x, y;
                    getInitialPosition(&x, &y, ENEMIES, positionNextEnemie);
                    //cria efeito do nascimento
                    fx_add(x+8, y+8, FX_TYPE_CREATION);
                    playSound(FX_TYPE_CREATION);
                }

                if(fx_finished(FX_TYPE_CREATION)){
                    crateEnemie(game.enemies, game.enemiesQuant, &positionNextEnemie);
                }

                GameObject_t explodedBlocks[10];
                int quantExplodedBlocks = updateMap(&game, explodedBlocks);
                for(int i = 0; i < quantExplodedBlocks; i++){
                    int x, y;
                    getMiddlePosition(explodedBlocks, i, &x, &y);
                    fx_add(x, y, FX_TYPE_EXPLOSION);
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
            drawInfo();

            //Exibe na tela
            showDraw();
            redraw = false;
        }
        cicle++;
        resetCicle(&cicle);
    }
    closeDisplay();
    state = 8;
}