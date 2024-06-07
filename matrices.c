#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1024

int MatrizA[MAX][MAX];
int MatrizB[MAX][MAX];
int MatrizC[MAX][MAX];

void rellenarMatriz(int **matriz, int rows, int cols){
    srand(time(NULL));
    for(int i=0; i<rows; i++){
        for(int j=0; j<cols; j++){
            matriz[i][j] = rand() % 10 + 1;
           
        }
    }
}

int main(){

    int **MatrizA, **MatrizB, **MatrizC;

    MatrizA = (int **)malloc(MAX * sizeof(int *));
    MatrizB = (int **)malloc(MAX * sizeof(int *));
    MatrizC = (int **)malloc(MAX * sizeof(int *));
    for (int i = 0; i < MAX; i++) {
        MatrizA[i] = (int *)malloc(MAX * sizeof(int));
        MatrizB[i] = (int *)malloc(MAX * sizeof(int));
        MatrizC[i] = (int *)malloc(MAX * sizeof(int));
    }

    rellenarMatriz(MatrizA, MAX, MAX);
    rellenarMatriz(MatrizB, MAX, MAX);

    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            MatrizC[i][j] = 0;
            for (int k = 0; k < MAX; k++) {
                MatrizC[i][j] += MatrizA[i][k] * MatrizB[k][j];
            }
        }
    }

    for (int i = 0; i < MAX; i++) {
        free(MatrizA[i]);
        free(MatrizB[i]);
        free(MatrizC[i]);
    }
    free(MatrizA);
    free(MatrizB);
    free(MatrizC);

    return 0;
}
