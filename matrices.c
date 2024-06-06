#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1024

int MatrizA[MAX][MAX];
int MatrizB[MAX][MAX];
int MatrizC[MAX][MAX];

void rellenarMatriz(){
    srand(time(NULL));
    for(int i=0; i<MAX; i++){
        for(int j=0; j<MAX; j++){
            MatrizA[i][j] = rand() % 10 + 1;
            MatrizB[i][j] = rand() % 10 + 1;
            MatrizC[i][j] = rand() % 10 + 1;
        }
    }
}

int main(int argc, char ** argv){
    rellenarMatriz();



}