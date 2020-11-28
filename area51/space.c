#include<stdio.h>
#include<stdlib.h>

#include "display.h"

#define KEY_SEEN     1
#define KEY_RELEASED 2

typedef struct{
    int x, y;
    int dx, dy;
    char sprite;
} obj;

void initObj(obj *objeto, char sprite){
    objeto->sprite = sprite;
    objeto->dx = 0;
    objeto->dy = 0;
    objeto->x = 0;
    objeto->y = 0;
}

void setPos(obj *objeto, int x, int y){
    objeto->x = x;
    objeto->y = y;
}

void setDelta(obj *objeto, int dx, int dy){
    objeto->dx = dx;
    objeto->dy = dy;
}

bool eventTimer(unsigned char key[ALLEGRO_KEY_MAX], obj *nave){
    bool done =  false;

    if(key[ALLEGRO_KEY_UP])
        nave->y--;
    if(key[ALLEGRO_KEY_DOWN])
        nave->y++;
    if(key[ALLEGRO_KEY_LEFT])
        nave->x--;
    if(key[ALLEGRO_KEY_RIGHT])
        nave->x++;

    if(key[ALLEGRO_KEY_ESCAPE])
        done = true;

    for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
        key[i] &= KEY_SEEN;
    
    return done;
}

int main (){
    initDisplay();
    obj nave;
    initObj(&nave, '^');
    
    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    startFPS();
    while(1){
        al_wait_for_event(queue, &event);
        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                done = eventTimer(key, &nave);
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
            drawDisplay(nave.x, nave.y, nave.sprite);
            al_flip_display();
            redraw = false;
        }
    }
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    return 0;
}
