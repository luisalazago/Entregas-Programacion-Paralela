/*
    Name: Entrega 2 Programación Paralela (OpenMPI V2).
    Date: 05/04/2022
    Authors: Guido Ernesto Salazar and Luis Alberto Salazar.
    Company: Pontificia Universidad Javeriana Cali.
*/

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

#define MSGTAG 0
#define FIL 10
#define COL 10
#define ROOT 0

void llenar(int matrix[FIL][COL], int *vector){
    /*
    Functions that initiallize the matrix and vector that will be used to
    calculate the dot product
    Inputs:
        Matrix: is a matrix of FILxCOL
        Vector: is a matrix of COLx1
    Outputs
        Matrix: is a matrix of FILxCOL
        Vector: is a matrix of COLx1

    */
    int i, j;
    for(i = 0; i < COL; ++i){
        vector[i] = i+1;
        for(j = 0; j < FIL; ++j)
            matrix[j][i] = i+j;
    }
}

int main(int argc, char *argv[]) {
    // The variables are created.
    int MSGSIZE;
    long long int suma;
    int my_rank, p, i, j;
    int matriz[FIL][COL], vector[COL]; //Matriz is a matrix FILxCOL with numbers and vector is matrix COLx1
    int vectorT[FIL*COL]; // vectorT is a vector that is use to pass the values through the process.
    int ans[FIL]; // is the matrix FILx1 restul of the dot prodcut between matriz*vector;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MSGSIZE = (int)ceil(((double)FIL / (p - 1))); // Rows per process;
    suma = 0;

    if (my_rank == ROOT) {
        printf("Corriendo entrega\n");
        // The vector is initialize
        llenar(matriz, vector);
    }

    //Divide the work between the difference process of the matriz size
    MPI_Scatter(matriz, MSGSIZE*COL, MPI_INT, vectorT, MSGSIZE*COL, MPI_INT, ROOT, MPI_COMM_WORLD); 

    //Send the values of the second vector to all the process
    MPI_Bcast(vector, COL, MPI_INT, ROOT, MPI_COMM_WORLD);
    
    //Calculate the Dot prodcut
    for(i = 0; i < MSGSIZE; ++i){
        //Do not calculate nothing if we are not in the size of the matrix
        if(my_rank*MSGSIZE+i<FIL){
            suma = 0;
            //Dot product
            #pragma omp parallel for private(j) reduction(+:suma)
            for(j = 0; j < COL; ++j){
                suma += vector[j] * vectorT[i*FIL+j];
            }
            //Saving the value
            ans[i] = suma;
        }
    }

    //Wait for all the process to do their work
    MPI_Barrier(MPI_COMM_WORLD);
    //Gather all the information on the root process
    MPI_Gather(ans, MSGSIZE, MPI_INT, ans, MSGSIZE, MPI_INT, ROOT, MPI_COMM_WORLD);

    if(my_rank == ROOT){
        printf("Dot product is:\n");
        for(i = 0; i < FIL; ++i)
            printf("[%d] = %d\n", i, ans[i]);
    }

    MPI_Finalize();
    return 0;
}
