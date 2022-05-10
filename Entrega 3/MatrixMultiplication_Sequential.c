#include <stdio.h>
#include <stdlib.h>

#define FIL1 10
#define FIL2 12
#define COL 8

void llenar(int **matrix, int fil, int col){
    /*
    Functions that initiallize the matrix that will be used to
    calculate the dot product
    Inputs:
        Matrix: is a matrix of FILxCOL
    Outputs
        Matrix: is a matrix of FILxCOL

    */
    int i, j;
    for(i = 0; i < fil; ++i){
        for(j = 0; j < col; ++j)
            matrix[i][j] = i+j;
    }
}

void calcular(int **matrix1, int **matrix2, int **matrixT) {
    int i, j, k, suma, temp;
    
    for(k = 0; k < FIL2; ++k) {
        for(i = 0; i < FIL1; ++i) {
            suma = 0;
            for(j = 0; j < COL; ++j) {
                suma += matrix1[i][j] * matrix2[j][k];
            }
            matrixT[i][k] = suma;
        }
    }
}

void imprimir(int **matrixT) {
    int i, j;
    printf("Result of the dot product stored in matrixT\n");
    
    for(i = 0; i < FIL1; ++i) {
        printf("[%d]:", i + 1);
        for(j = 0; j < FIL2; ++j) {
            printf(" %d", matrixT[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int **matriz1, **matriz2;
    int **matrizT;
    int i;
    
    matriz1 = (int**) malloc(FIL1 * sizeof(int*));
    matriz2 = (int**) malloc(COL * sizeof(int*));
    matrizT = (int**) malloc(FIL1 * sizeof(int*));
    for(i = 0; i < FIL1; ++i) {
        matriz1[i] = (int*) malloc(COL * sizeof(int));
    }
    for(i = 0; i < COL; ++i) {
        matriz2[i] = (int*) malloc(FIL2 * sizeof(int));
    }
    for(i = 0; i < FIL1; ++i) {
        matrizT[i] = (int*) malloc(FIL2 * sizeof(int));
    }
    
    llenar(matriz1, FIL1, COL);
    llenar(matriz2, COL, FIL2);
    
    calcular(matriz1, matriz2, matrizT);
    imprimir(matrizT);
    
    return 0;
}
