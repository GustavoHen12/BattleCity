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
    GameObject_t tank = initGameObject(10, 10, 1, 1, TANK);
    InitGame();
    int input = -1;
    int cicle = 0;
    //laço principal
    bool done = false;
    bool redraw = true;
    TimerEvent_t event;
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
    startFPS();
    while(1){
        al_wait_for_event(queue, &event);
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                //movimenta tank
                input = readInput(key);
                if(input != -1)
                    move(&tank, input);
                //atira
                if(key[ALLEGRO_KEY_Z])
                    shoot(&tank);
                //fecha jogo
                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;
                nextCicle(&cicle);
                updateEnemies();
                //movGame();
                //colisionDetection();
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
            beforeDraw();
            //desenha tank
            drawDisplay(tank.x, tank.y, tank.type, tank.direction);
            //desenha inimigos
            for(int i = 0; i < game.enemiesSize; i++){
                GameObject_t enemie;
                enemie = game.enemies[i];
                drawDisplay(enemie.x, enemie.y, enemie.type, enemie.direction);
            }
            //desenha tiro
            for(int i = 0; i < game.shootsSize; i++){
                GameObject_t shot;
                shot = game.shoots[i];
                drawDisplay(shot.x, shot.y, shot.type, shot.direction);
            }
            showDraw();
            redraw = false;
        }
        cicle++;
    }
    closeDisplay();
    state = 8;
}