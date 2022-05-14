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

void calcular(int *matriz1, int *matriz2, int *ans, int MSGSIZE1, int MSGSIZE2, int my_rank) {
   int i, j, k, suma;
   for(i = 0; i < MSGSIZE1; ++i) {
      for(j = 0; j < MSGSIZE2; ++j){
        if(my_rank*MSGSIZE2+j<FIL2){
          suma = 0;
          for(k = 0; k < COL; ++k){
            suma += matriz1[i*COL+k] * matriz2[j*COL+k];
          }
          ans[j] = suma;
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
    int MSGSIZE1, MSGSIZE2;
    int my_rank, p, i, j, fil_root, index;

    int *vectorT1, *vectorT2; // vectorT is a vector that is use to pass the values through the process.
    int *temp1,*temp2;
    int ans[FIL1*FIL2]; // is the matrix FILx1 restul of the dot prodcut between matriz*vector;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MSGSIZE1 = (int)ceil(((double)FIL1 /(double)(p))); // Rows per process of matrix1
    MSGSIZE2 = (int)ceil(((double)FIL2 /(double)(p))); // Colums pero process of matrix2

    //matriz2 = (int*) malloc((FIL2*COL) * sizeof(int));
    //ans = (int*) malloc((MSGSIZE1*FIL2) * sizeof(int));
    temp1 = (int*) malloc(COL * sizeof(int));
    vectorT1 = (int*) malloc((MSGSIZE1*COL) * sizeof(int));
    vectorT2 = (int*) malloc((MSGSIZE2*COL) * sizeof(int));

    if (my_rank == ROOT) {
        printf("Corriendo entrega\n");
        //matriz1 = (int*) malloc((FIL1*COL) * sizeof(int));
        //matrizT = (int*) malloc((FIL1*FIL2) * sizeof(int));

        llenar(matriz1, FIL1, COL);
        llenar(matriz2, FIL2, COL);
        printf("MSGSIZE1=%d | MSGSIZE2=%d\n", MSGSIZE1, MSGSIZE2);
        //imprimir(matriz1, FIL1, COL, "Matrix1");
        //imprimir(matriz2, COL, FIL2, "Matrix2");
    }

    //Divide the work between the difference process of the matriz1
    MPI_Scatter(matriz1, MSGSIZE1*COL, MPI_INT, vectorT1, MSGSIZE1*COL, MPI_INT, ROOT, MPI_COMM_WORLD);

    //Divide the work between the difference process of the matriz2
    MPI_Scatter(matriz2, MSGSIZE2*COL, MPI_INT, vectorT2, MSGSIZE2*COL, MPI_INT, ROOT, MPI_COMM_WORLD);


    //Calculate the Dot prodcut

    //for(i = 0; i < p; ++i){
      //if(my_rank == i){
        //printf("%d: is the process id for broadcast\n", i);
        //for(j = 0; j < MSGSIZE1*COL; ++j)
          //temp1[j] = vectorT1[j];
      //}
      //MPI_Bcast(temp1, FIL1*COL, MPI_INT, i, MPI_COMM_WORLD);
      //calcular(temp1, vectorT2, temp2, MSGSIZE1, MSGSIZE2, my_rank);
      //MPI_Barrier(MPI_COMM_WORLD);
      //MPI_Gather(temp2, MSGSIZE2, MPI_INT, ans, MSGSIZE2, MPI_INT, i, MPI_COMM_WORLD);
      //if(my_rank == i)
        //printf("%d: is the process id for gather\n", i);
    //}

    for(i = 0; i < FIL1; ++i){
      temp2 = (int*) malloc(MSGSIZE2 * sizeof(int));
      fil_root = i/MSGSIZE1;
      index = i%MSGSIZE1;
      if(my_rank == fil_root){
        for(j = 0; j < COL; ++j)
          temp1[j] = vectorT1[index*COL+j];
      }

      MPI_Bcast(temp1, COL, MPI_INT, fil_root, MPI_COMM_WORLD);
      calcular(temp1, vectorT2, temp2, 1, MSGSIZE2, my_rank);
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Gather(temp2, MSGSIZE2, MPI_INT, ans+(index*FIL2), MSGSIZE2, MPI_INT, fil_root, MPI_COMM_WORLD);
      free(temp2);
    }

    //Wait for all the process to do their work
    MPI_Barrier(MPI_COMM_WORLD);

    //Gather all the information on the root process
    MPI_Gather(ans, MSGSIZE1*FIL2, MPI_INT, matrizT, MSGSIZE1*FIL2, MPI_INT, ROOT, MPI_COMM_WORLD);

    if(my_rank == ROOT){
        imprimir(matrizT, FIL1, FIL2, "MatrixT");
    }

    //free(matriz1);
    //free(matriz2);
    //free(matrizT);
    //free(ans);
    free(vectorT1);
    free(temp1);
    free(vectorT2);

    MPI_Finalize();

    return 0;
}
