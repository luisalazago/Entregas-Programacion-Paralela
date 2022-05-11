#include <stdio.h>
#include <stdlib.h>

#define FIL1 10
#define FIL2 12
#define COL 8

void llenar(int *matrix, int fil, int col){
    /*
    Functions that initiallize the matrix that will be used to
    calculate the dot product
    Inputs:
        Matrix: is a matrix of FILxCOL
    Outputs
        Matrix: is a matrix of FILxCOL

    */
    int i, j;
    for(i = 0; i < fil; ++i) {
        for(j = 0; j < col; ++j) {
          matrix[(i*col)+j] = i+j;
        }
    }
}

void calcular(int *matrix1, int *matrix2, int *matrixT) {
    int i, j, k, suma;

    for(i = 0; i < FIL1; ++i) {
        for(k = 0; k < FIL2; ++k) {
            suma = 0;
            for(j = 0; j < COL; ++j) {
                suma += matrix1[(i*COL)+j] * matrix2[(j*FIL2)+k];
            }
            matrixT[i*FIL2+k] = suma;
        }
    }
}

void imprimir(int *matrix, int fil, int col, char *pal) {
    int i, j;
    printf("%s\n", pal);
    if(fil == FIL1 && col == FIL2)
      printf("Result of the dot product stored in matrixT\n");

    else
      printf("Values store\n");
    for(i = 0; i < fil; ++i) {
        printf("[%d]:", i + 1);
        for(j = 0; j < col; ++j) {
            printf(" %d", matrix[(i*col)+j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int *matriz1, *matriz2;
    int *matrizT;
    int i;

    matriz1 = (int*) malloc(FIL1*COL * sizeof(int));
    matriz2 = (int*) malloc(COL*FIL2 * sizeof(int));
    matrizT = (int*) malloc(FIL1*FIL2 * sizeof(int));

    llenar(matriz1, FIL1, COL);
    llenar(matriz2, COL, FIL2);

    calcular(matriz1, matriz2, matrizT);
    imprimir(matrizT, FIL1, FIL2, "MatrixT");

    return 0;
}
