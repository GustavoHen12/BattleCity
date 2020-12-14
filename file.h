#ifndef __FILE_TXT__
#define __FILE_TXT__

#include<stdlib.h>
#include<stdio.h>

FILE *openFile(char *filename, char *mode);

void saveScore(int score);

void getTopScore(int *topScore, int *lastScore);

#endif