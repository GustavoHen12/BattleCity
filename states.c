#include "states.h"
//todo: achar um lugar melhor pra isso
#define KEY_SEEN     1
#define KEY_RELEASED 2

void initGame(){
    initDisplay();
}

void start(){
    initGame();
    state = PLAY;
}

void play(){
    //instancia variaveis
    printf("PLAY \n");
    // //laço principal
    // bool done = false;
    // bool redraw = true;
    // TimerEvent_t event;

    // unsigned char key[ALLEGRO_KEY_MAX];
    // memset(key, 0, sizeof(key));
    // startFPS();
    // while(1){
    //     al_wait_for_event(queue, &event);
    //     switch(event.type)
    //     {
    //         case ALLEGRO_EVENT_TIMER:
    //             //movTank(key, &nave);
    //             //movGame();
    //             //colisionDetection();
    //             redraw = true;
    //             break;
            
    //         case ALLEGRO_EVENT_KEY_DOWN:
    //             key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
    //             break;

    //         case ALLEGRO_EVENT_KEY_UP:
    //             key[event.keyboard.keycode] &= KEY_RELEASED;
    //             break;
            
    //         case ALLEGRO_EVENT_DISPLAY_CLOSE:
    //             done = true;
    //             break;
    //     }

    //     if(done)
    //         break;
        
    //     if(redraw && al_is_event_queue_empty(queue)){
    //         beforeDraw();
    //         //drawDisplay(nave.x, nave.y, nave.sprite);
    //         showDraw();
    //         redraw = false;
    //     }
    // }
}