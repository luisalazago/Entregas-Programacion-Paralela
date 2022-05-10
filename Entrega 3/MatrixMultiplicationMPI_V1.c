#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define FIL1 10
#define FIL2 12
#define COL 8
#define ROOT 0

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
    int MSGSIZE;
    long long int suma;
    int my_rank, p, i, j, k;
    int *vectorT; // vectorT is a vector that is use to pass the values through the process.
    int ans[FIL1][FIL2]; // is the matrix FILx1 restul of the dot prodcut between matriz*vector;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MSGSIZE = (int)ceil(((double)FIL1 / (p))); // Rows per process;
	suma = 0;
    
    matriz2 = (int**) malloc(COL * sizeof(int*));
    vectorT = (int*) malloc((MSGSIZE*COL) * sizeof(int));
    for(i = 0; i < COL; ++i) {
        matriz2[i] = (int*) malloc(FIL2 * sizeof(int));
    }
	if (my_rank == ROOT) {
        printf("Corriendo entrega\n");
		matriz1 = (int**) malloc(FIL1 * sizeof(int*));
        matrizT = (int**) malloc(FIL1 * sizeof(int*));
        for(i = 0; i < FIL1; ++i) {
            matriz1[i] = (int*) malloc(COL * sizeof(int));
        }
        for(i = 0; i < FIL1; ++i) {
            matrizT[i] = (int*) malloc(FIL2 * sizeof(int));
        }
    
		llenar(matriz1, FIL1, COL);
        llenar(matriz2, FIL2, COL);
    }
    

    //Divide the work between the difference process of the matriz size
    MPI_Scatter(matriz1, MSGSIZE*COL, MPI_INT, vectorT, MSGSIZE*COL, MPI_INT, ROOT, MPI_COMM_WORLD);	

    //Send the values of the matriz2 to all the process
	MPI_Bcast(matriz2, FIL2*COL, MPI_INT, ROOT, MPI_COMM_WORLD);
    
    //Calculate the Dot prodcut
    for(k = 0; k < FIL2; ++k) {
        for(i = 0; i < MSGSIZE; ++i){
            //Do not calculate nothing if we are not in the size of the matrix
            if(my_rank*MSGSIZE+i<FIL1){
                suma = 0;
                //Dot product
                for(j = 0; j < COL; ++j) {
                    suma += matriz2[k][j] * vectorT[i*FIL1+j];
                }
                //Saving the value
                ans[i][k] = suma;
            }
        }
    }

    //Wait for all the process to do their work
    MPI_Barrier(MPI_COMM_WORLD);
    //Gather all the information on the root process
    MPI_Gather(ans, MSGSIZE*FIL2, MPI_INT, ans, MSGSIZE*FIL2, MPI_INT, ROOT, MPI_COMM_WORLD);

    if(my_rank == ROOT){
        printf("Dot product is:\n");
        for(i = 0; i < FIL1; ++i) {
            printf("[%d]:", i + 1);
            for(j = 0; j < FIL2; ++j) {
                printf(" %d", ans[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    
    return 0;
}
