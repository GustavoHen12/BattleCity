#include "states.h"

void initGame(){
    initDisplay();
}

void start(){
    initGame();
    state = INIT_STAGE;
}

void init_stage(){
    state = PLAY;
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
void play(){
    //instancia variaveis
    InitGame();
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
                if(key[ALLEGRO_KEY_Z]) shoot(&game.tank);

                updateShot();

                if(updateEnemies()){
                    fx_add(game.shots[0].x + 5, game.shots[0].y + 5); //TODO: Arrumar posição explosão
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
            // //desenha tank
            beforeDraw();
            drawDisplay(game.tank.x, game.tank.y, game.tank.type, game.tank.direction);
            //desenha inimigos
            for(int i = 0; i < game.enemiesQuant; i++){
                GameObject_t enemie;
                enemie = game.enemies[i];
                drawDisplay(enemie.x, enemie.y, enemie.type, enemie.direction);
            }
            //desenha tiro
            for(int i = 0; i < game.shotsQuant; i++){
                GameObject_t shot;
                shot = game.shots[i];
                drawDisplay(shot.x, shot.y, shot.type, shot.direction);
            }
            for(int i = 0; i < game.mapQuant; i++){
                Wall_t wall = game.map[i];
                for(int j = 0; j < wall.quantBlock; j++){
                    GameObject_t block;
                    block = wall.blocks[j];
                    drawDisplay(block.x, block.y, block.type, block.direction);
                }
            }
            //desenha efeitos
            fx_draw();
            showDraw();
            redraw = false;
        }
        cicle++;
    }
    closeDisplay();
    state = 8;
}