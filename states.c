#include "states.h"

// ------------ Funções auxiliares ------------

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
    // Configurações iniciais
    beforeDraw();
    
    //TODO: Desenha borda e conteudos estáticos
    
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
        Wall_t wall = game->map[i];
        for(int j = 0; j < wall.quantBlock; j++){
            drawDisplay(&wall.blocks[j]);
        }
    }

    //Desenha efeitos
    fx_draw();

    //Exibe na tela
    showDraw();
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
                if(key[ALLEGRO_KEY_Z]) shoot(&game.tank, 3);

                int deadEnemie = updateEnemies(&game); 
                if(deadEnemie >= 0){
                    int x, y;
                    getMiddlePosition(game.enemies, deadEnemie, &x, &y);
                    fx_add(x, y);
                }

                int explodedShots = updateShots(&game); 
                if(explodedShots >= 0){
                    int x, y;
                    getMiddlePosition(game.shots, explodedShots, &x, &y);
                    fx_add(x, y);
                }

                int quantExplodedBlocks = 0;
                GameObject_t explodedBlocks[10];
                quantExplodedBlocks = updateMap(&game, explodedBlocks);
                if(explodedBlocks >= 0){
                    for(int i = 0; i < quantExplodedBlocks; i++){
                        int x, y;
                        getMiddlePosition(explodedBlocks, i, &x, &y);
                        fx_add(x, y);
                    }
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
            drawGame(&game);
            redraw = false;
        }
        cicle++;
    }
    closeDisplay();
    state = 8;
}