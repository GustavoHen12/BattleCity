#ifndef __GAME__
#define __GAME__
/*
* A "game" define as funções da lógica dos elemtos do jogo
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#include"gameBase.h"
#include"file.h"

#define MAX_SHOOT_SIZE 200
#define INTERVAL_GENERATE_ENEMIES 180
#define ENEMIES_QUANT 4
#define SHOTS_QUANT 5
#define TANK_LIFES 3
#define TANK_SHOT_INDEX (SHOTS_QUANT-1)

enum type_wall {BRICK, STONE, BUSH};
/*
* Struct para armazenar os dados de uma parede do jogo
*/
typedef struct{
    GameObject_t *blocks;
    int x, y, height, width, quantBlock, type;
} Wall_t;

/*
* Struct que representa todos os elementos do jogo 
*/
typedef struct{
    //tanque
    GameObject_t tank;
    
    //Mapa e quantidade de paredes
    Wall_t *map;
    int mapQuant;
    
    //Lista de elementos de tamanho "ENEMIES_QUANT" 
    //para os tanques inimigos
    GameObject_t *enemies;
    int enemiesQuant;

    //Lista de elementos de tamanho "SHOTS_QUANT" 
    //para os tiros dos inimigos (as primeiras posiçõs)
    //e do tanque (última posição do vetor)
    GameObject_t *shots;
    int shotsQuant;

    //Águia (ou brasão) central do jogo
    GameObject_t eagle;
} Game_t;

/*
* Nesta struct são armazenados os dados significativos para processar
* um ciclo do jogo:
*/
typedef struct{
    // Direção (definida em gameObject.c), da entrada do usuário
    int input;
    // Se o usuário tenta disparar um tiro
    int shot;
    // Os tanques inimigos podem nascer em três posições
    // que se repetem ciclicamente, nesta variável é armazenada qual a 
    // próxima posição
    int positionNextEnemy;
    // Como algumas ações são tomadas ciclicamente é necessário guardar o ciclo
    // cada ciclo dura 3seg, ou 180frames
    int cicle;
} ProcessGameInfo_t;

/*
* Struct para armazenar os dados do jogo que são exibidos para o jogador
*/
typedef struct{
    // Quantas vidas o jogador possui
    int life;
    // Pontuação
    int score;
    // Peso do ponto
    int pointsPerKill;
    // Quantidade de inimigos restante
    int enemiesRemaining;
} GameData_t;


//Inicia os objetos em "game" para uma nova partida
void initGame(Game_t *game);

int verifyGameOver(Game_t *game, GameData_t *data);

/*
* As funções mais significantes são as de update, pois ela contem a lógica
* de cada elemento do jogo, ou seja, com quem ele colide, se atira ou não
* se altera posição...
*/

// ------------ Tanque ------------
//Tenta mover o tanque em direction e verifica se
//ele foi atingido por algum tiro do inimigo
int updateTank(Game_t *game, int direction);

// ------------ Tiro ------------
/*
* Como a lista de tiros do tanque e dos inimigos é a mesma, a diferenciação
* é feita a partir do indice do tiro
* Para os inimigos, o inidice do tiro é igual a sua posição no vetor "enemies"
* O tiro do tanque é o último da lista, definido em TANK_SHOT_INDEX
* Se o atirador já possui um tiro no jogo, este não é feito
*/
int shoot(GameObject_t *shooter, GameObject_t *shots, int index);
/*
* Movimenta o tiro e verifica se ele saiu da tela
* Os tiros que sairam da tela são retornados em "exploded" e 
* a função retorna a quantidade
*/
int updateShots(Game_t *game, GameObject_t *exploded);

// ------------ Inimigos ------------
/*
* A movimentação do inimigos é realizada sempre tentando mover ele
* na mesma direção, se possui um obstáculo ele escolhe aleatoriamente uma nova
* e, com relação ao tira, ele sempre tenta atirar.
*/
int updateEnemies(Game_t *game);

/*
* adiciona inimigo ao jogo
*/
int crateEnemie(GameObject_t *enemies, int quant, int *position);

/*
* verifica o ciclo e o estado atual do jogo para decidir seé necessário ou não
* adicionar mais um tanque inimigo ao campo do jogo
*/
int sendEnemy(Game_t *game, int cicle, int enemiesRemaining);

// ------------ Mapa ------------
/*
* Lê o arquivo config.txt em resources e inicia o mapa
*/
void initMap();

/*
* Verifica se algum tiro atingiu uma parede
* A função retorna a quantidade de blocos retirados,
* e eles são retornado no vetor "exploded"
*/
int updateMap(Game_t *game, GameObject_t *exploded);


// ------------ GameData ------------
/*
* Aumenta pontuação de acordo com o peso do ponto no momento
*/
void increaseScore (GameData_t *data);

/*
* diminui a vida
*/
void decraseLife (GameData_t *data);

/*
* diminui a quantidade de inimigos restantes
*/
void decreaseEnemiesRemainig(GameData_t *data);

/*
* diminui o peso do ponto
*/
void decrasePointsPerKill(GameData_t *data);

#endif