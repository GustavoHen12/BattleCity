#include<stdio.h>
#include"states.h"

int main(){
    state = START;
    for(;;){
        switch (state)
        {
            case START:
                start();
                break;
            case INIT_STAGE:
                init_stage();
                break;
            case PLAY:
                play();
                break;
            case GAME_OVER:
                //gameOver();
                break;
            case LEFT_GAME:
                //leftGame();
                break;
            default:
                break;
        }
    }
}