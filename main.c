#include<stdio.h>
#include"states.h"

void imprime_vetor (int v[], int tam) {
    int i;

    for (i=0; i<tam-1; i++)
        printf("%d ",v[i]);
    printf("%d\n",v[tam-1]);
}


int main(){
    state = START;
    int end = 0;
    int vet[5], l;
    getTopScore(vet, &l);
    printf("%d \n", l);
    imprime_vetor(vet, 5);
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
                end = 1;
                break;
            default:
                break;
        }
        if(end)
            break;
    }
}