#include"file.h"

/*
* InserctionSort para ordenar os valores do vetor
*/
void inserctionSort (int v[], int ini, int fim){
    int i, j, chave;
    for (i = ini+1; i <= fim; i++)
    {
        chave = v[i];
        j = i-1;
        while((chave > v[j]) && (j > -1)){
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = chave;
    }
}

/*
* Abre arquivo "filename", no modo "mode" e retorna ponteiro
*/
FILE *openFile(char *filename, char *mode){
    FILE* arq;
    //abre arquivo
	arq = fopen(filename, mode);
    //verifica se funcionou, ou seja, se é um ponteiro válido
    if(!arq){
        fprintf(stderr, "[ERRO]: Não foi possível abrir o arquivo %s.\n", filename);
        exit(1);
    }

    return arq;
}

void saveScore(int score){
    //abre o arquivo
    FILE *arq = openFile("resources/score.txt", "a");
    //tenta salvar o score na ultima linha, se não conseguir exibe msg de erro
    if(fprintf (arq, "%d\n", score) < 0){
        fprintf(stderr, "[ERRO INTERNO]: Não foi possível salvar o score %d.\n", score);
        exit(1);
    }
}

void getTopScore(int *topScore, int *lastScore){
    //abre o arquivo
    FILE *arq = openFile("resources/score.txt", "r");
    int num = 0, size = 0, count = 1;
    int *vet;
    vet = malloc(sizeof(int) * 100);
	//Salva todos os valores no vetor "vet"
    while(!feof(arq))
	{
		fscanf(arq, "%d", &num);
        vet[size] = num;
        size++;
        if(size > (count*100)){
            count++;
            vet = realloc(vet, sizeof(int)*100*count);
        }
	}
    //Passa o ultimo valor salvo para "lastscore0"
    *lastScore = vet[size-1];

    //Ordena o vetor
    inserctionSort(vet, 0, size-1);
    //passa os 5 primeiros valores para o top score
    for(int i = 0; i < 5; i++){
        topScore[i] = vet[i];
    }
}