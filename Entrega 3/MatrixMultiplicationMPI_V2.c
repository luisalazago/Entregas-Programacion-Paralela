#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define FIL1 10
#define FIL2 12
#define COL 8
#define ROOT 0

void llenar(int *matrix, int fil, int col) {
    /*
    Functions that initiallize the matrix that will be used to
    calculate the dot product

    Inputs:
    -------
    Matrix : vector<int>
      is an empty matrix of size filxcol represent as a vector
    fil : int
      number of rows of the matrix
    col : int
      number of columns of the matrix

    Outputs
    -------
    Matrix : vector<int>
      is a matrix of size FILxCOL fill with a_{i,j} = i+j
    */
    int i, j;
    for(i = 0; i < fil; ++i) {
      for(j = 0; j < col; ++j) {
        matrix[(i*col)+j] = i+j;
      }
    }
}

void calcular(int *matriz1, int *matriz2, int *ans, int MSGSIZE1, int MSGSIZE2, int my_rank) {
    /*
    Functions that calculates the dot product of two matrix

    Inputs:
    -------
    matriz1 : vector<int>
      is a vector of size MSGSIZE1xCOL represent as a vector
    matriz2 : vector<int>
      is a matrix of size MSGSIZE2xCOL represent as a vector notice that is
      asume that the matrix2 is the transpose of the original
    ans : vector<int>
      is an empty vector of size MSGSIZE2
    MSGSIZE1 : int
      is the rows of the first vector
    MSGSIZE2 : int
      is the rows of the second matrix
    my_rank : int
      is the id of the process that is working at the moment, in order to know whether is necesary to compute
      or the program is done

    Outputs
    -------
    ans : vector<int>
      is a vector of size 1xMSGSIZE2 fill with the result of the dot  product between matriz1 and matriz2
    */
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
    /*
    Procedure that prints what is inside of the matrix

    Inputs:
    -------
    matrix : vector<int>
      is a matrix of size filxcol represent as a vector
    fil : int
      is the rows of the matrix
    col : int
      is the colums of the matrix
    pal : char
      is a message that will be printed before the result
    */
    int i, j;
    printf("%s\n", pal);
    for(i = 0; i < fil; ++i) {
        printf("[%d]:", i + 1);
        for(j = 0; j < col; ++j) {
            printf(" %d", matrix[(i*col)+j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int matriz1[FIL1*COL], matriz2[COL*FIL2]; //Input matrix (Notice that matriz2 is asume to be the transpose. This is made for improvments in accesing the memory)
    int matrizT[FIL1*FIL2]; // Output matrix
    int MSGSIZE1, MSGSIZE2; // How many rows and colums will be in charge of the p process
    int my_rank, p, i, j, fil_root, index; //Other variables

    int *vectorT1, *vectorT2; // vectorT1 is a vector that is use to store the rows of the matriz1 that the diferent p process will be in charge, and vectorT2 is the same but for the matrix2.
    int *temp1,*temp2; // vectors that will be necesarry to compute the informtation
    int ans[FIL1*FIL2]; // is the matrix that will be gather the information of the process;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MSGSIZE1 = (int)ceil(((double)FIL1 /(double)(p))); // Rows per process of matrix1
    MSGSIZE2 = (int)ceil(((double)FIL2 /(double)(p))); // Colums pero process of matrix2

    //Allocating memory

    //matriz2 = (int*) malloc((FIL2*COL) * sizeof(int));
    //ans = (int*) malloc((MSGSIZE1*FIL2) * sizeof(int));
    temp1 = (int*) malloc(COL * sizeof(int));
    vectorT1 = (int*) malloc((MSGSIZE1*COL) * sizeof(int));
    vectorT2 = (int*) malloc((MSGSIZE2*COL) * sizeof(int));

    //FIlling the information for the root process
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

    for(i = 0; i < FIL1; ++i){
      //Allocating memory for the row that will be compute
      temp2 = (int*) malloc(MSGSIZE2 * sizeof(int));

      //Serching of the process that has the i row of the original matrix and the position.
      fil_root = i/MSGSIZE1;
      index = i%MSGSIZE1;

      //Allocating the information on the temp1 vector
      if(my_rank == fil_root){
        for(j = 0; j < COL; ++j)
          temp1[j] = vectorT1[index*COL+j];
      }

      //Sending the row to all process
      MPI_Bcast(temp1, COL, MPI_INT, fil_root, MPI_COMM_WORLD);

      //Calculate the dot prodcut
      calcular(temp1, vectorT2, temp2, 1, MSGSIZE2, my_rank);

      //Wait for all process to finish
      MPI_Barrier(MPI_COMM_WORLD);

      //Gather the information
      MPI_Gather(temp2, MSGSIZE2, MPI_INT, ans+(index*FIL2), MSGSIZE2, MPI_INT, fil_root, MPI_COMM_WORLD);

      //Free the memory
      free(temp2);
    }

    //Wait for all the process to do their work
    MPI_Barrier(MPI_COMM_WORLD);

    //Gather all the information on the root process
    MPI_Gather(ans, MSGSIZE1*FIL2, MPI_INT, matrizT, MSGSIZE1*FIL2, MPI_INT, ROOT, MPI_COMM_WORLD);

    //Printing the information
    if(my_rank == ROOT){
        imprimir(matrizT, FIL1, FIL2, "Result of the MatrixT");
    }

    //Free the memory
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
