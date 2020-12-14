#ifndef __GAME_BASE__
#define __GAME_BASE__
/*
* A gameBase define as funções que são comuns no jogo, ou seja, 
* as funções mais básicas e elementares ao jogo
*/

#include"gameObject.h"

/*
* Inicia e retorna um elemento do jogo do tipo "type", com posição (x, y) 
* velocidade dx, dy e tamanho (heigh, widht)
*/
GameObject_t initGameObject(int x, int y, int dx, int dy, int type, int heigh, int widht);

/*
* Retorna em x, y a posição inicial do elemento do jogo do tipo "type"
* Como os inimigos podem nascer em 3 posições diferente, caso seja um inimigo, 
* é passado em "position" qual das três posições é a selecionada
*/
void getInitialPosition(int *x, int *y, int type, int position);

/*
* Faz a cópia de um GameObject_t "source", para um "dest"
*/
void copyGameObject(GameObject_t *dest, GameObject_t *source);

/*
* Altera a posição de "objeto" para (x, y)
*/
void setPosition(GameObject_t *objeto, int x, int y);

/*
* Retorna 1 se o "obj" esta vivo e 0 caso contrário
*/
int isAlive(GameObject_t *obj);

/*
* A partir de uma lista de elemtos "obj", retorna a posição do centro do
* de indice "index" em (x, y)
*/
void getMiddlePosition(GameObject_t *obj, int index, int *x, int *y);

/*
* Reinicia um "obj"
* Caso seja do tipo tank apenas é colocado na posição inicial
* Caso seja um tanque inimigo é colocado em uma das três posições iniciais
* definida em "position" e têm sua vida setada para 1
*/
void respawn(GameObject_t *obj, int position);

/*
* softKill apenas diminui 1 da vida do "obj" 
*/
void softKill(GameObject_t *object);

/*
* A hardkill mata um elemento na lista "objectList" trocando de posição
* o elemento que será morto com o da última posição e retornando o novo tamanho
* de "objectList"
*/
int hardKill(GameObject_t *objectList, int size, int index);

/*
* Função retorna 1 se o objetoA esta colidindo com objeto B 
* (créditos: ver README)
*/
int colision(int xA, int yA, int heightA, int widhtA, int xB, int yB, int heightB, int widthB);

/*
* Esta função recebe um objeto e uma direção, e atualiza a
* direção e a velocidade do objeto
*/
void updateDirection(GameObject_t *object, int newDirection);

/*
* Movimenta o "object" em "direction"
*/
void move(GameObject_t *object, int direction);
#endif