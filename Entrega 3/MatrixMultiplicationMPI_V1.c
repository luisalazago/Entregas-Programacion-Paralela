#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define FIL1 10
#define FIL2 12
#define COL 8
#define ROOT 0

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
    for(i = 0; i < fil; ++i){
        for(j = 0; j < col; ++j) {
          matrix[(i*col)+j] = i+j;
        }
    }
}

void calcular(int *vectorT, int *matriz2, int *ans, int MSGSIZE, int my_rank) {
   int i, j, k, suma;
   for(i = 0; i < MSGSIZE; ++i) {
      if(my_rank*MSGSIZE+i<FIL1) {
        for(k = 0; k < FIL2; ++k) {
            //Do not calculate nothing if we are not in the size of the matrix
            suma = 0;
            //Dot product
            for(j = 0; j < COL; ++j) {
                suma += matriz2[j*FIL2+k] * vectorT[i*COL+j];
            }
                //Saving the value
            ans[i*(FIL2)+k] = suma;
            }
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
    int matriz1[FIL1*COL], matriz2[COL*FIL2];
    int matrizT[FIL1*FIL2];
    int MSGSIZE;
    int my_rank, p;
    int *vectorT; // vectorT is a vector that is use to pass the values through the process.
    int *ans; // is the matrix FILx1 restul of the dot prodcut between matriz*vector;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MSGSIZE = (int)ceil(((double)FIL1 /(double)(p))); // Rows per process;

    //matriz2 = (int*) malloc((FIL2*COL) * sizeof(int));
    vectorT = (int*) malloc((MSGSIZE*COL) * sizeof(int));
    ans = (int*) malloc((MSGSIZE*FIL2) * sizeof(int));

    if (my_rank == ROOT) {
        printf("Corriendo entrega\n");
        //matriz1 = (int*) malloc((FIL1*COL) * sizeof(int));
        //matrizT = (int*) malloc((FIL1*FIL2) * sizeof(int));

        llenar(matriz1, FIL1, COL);
        llenar(matriz2, COL, FIL2);

        //imprimir(matriz1, FIL1, COL, "Matrix1");
        //imprimir(matriz2, COL, FIL2, "Matrix2");
    }

    //Divide the work between the difference process of the matriz size
    MPI_Scatter(matriz1, MSGSIZE*COL, MPI_INT, vectorT, MSGSIZE*COL, MPI_INT, ROOT, MPI_COMM_WORLD);

    //Send the values of the matriz2 to all the process
    MPI_Bcast(matriz2, FIL2*COL, MPI_INT, ROOT, MPI_COMM_WORLD);

    //Calculate the Dot prodcut
    calcular(vectorT, matriz2, ans, MSGSIZE, my_rank);

    //Wait for all the process to do their work
    MPI_Barrier(MPI_COMM_WORLD);

    //Gather all the information on the root process
    MPI_Gather(ans, MSGSIZE*FIL2, MPI_INT, matrizT, MSGSIZE*FIL2, MPI_INT, ROOT, MPI_COMM_WORLD);

    if(my_rank == ROOT){
        imprimir(matrizT, FIL1, FIL2, "MatrixT");
    }

    //free(matriz1);
    //free(matriz2);
    //free(matrizT);
    free(vectorT);
    free(ans);

    MPI_Finalize();

    return 0;
}
