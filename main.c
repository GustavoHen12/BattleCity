#include<stdio.h>
#include"states.h"

enum{ START, INIT_STAGE, PLAY, GAME_OVER, LEFT_GAME } state;

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
                gameOver();
                break;
            case LEFT_GAME:
                leftGame();
                break;
            default:
                break;
        }
    }
}