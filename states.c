#include "states.h"

void initGame(){
    initDisplay();
}

void start(){
    initGame();
    state = PLAY;
}

void play(){
    //instancia variaveis
    GameObject_t tank = initGameObject(10, 10, TANK);
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
                if(key[ALLEGRO_KEY_LEFT])
                    setPosition(&tank, tank.x-1, tank.y);
                if(key[ALLEGRO_KEY_RIGHT])
                    setPosition(&tank, tank.x+1, tank.y);
                if(key[ALLEGRO_KEY_UP])
                    setPosition(&tank, tank.x, tank.y-1);
                if(key[ALLEGRO_KEY_DOWN])
                    setPosition(&tank, tank.x, tank.y+1);

                if(key[ALLEGRO_KEY_ESCAPE])
                    done = true;

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;
                //movTank(key, &nave);
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
            drawDisplay(tank.x, tank.y, tank.type);
            showDraw();
            redraw = false;
        }
    }
    closeDisplay();
    state = 8;
}