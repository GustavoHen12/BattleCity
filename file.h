#ifndef __FILE_TXT__
#define __FILE_TXT__

#include<stdlib.h>
#include<stdio.h>


FILE *openFile(char *filename, char *mode);

/*
* Salva score
*/
void saveScore(int score);

/*
* Retorna os 5 primeiros scores, e o score mais recente
*/
void getTopScore(int *topScore, int *lastScore);

#endif