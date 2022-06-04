/*
    Name: Entrega 1 Programación Paralela.
    Date: 02/03/2022
    Authors: Guido Ernesto Salazar and Luis Alberto Salazar.
    Company: Pontificia Universidad Javeriana Cali.
*/

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

#define MSGTAG 0
#define TAM 10

void llenar(int *vector1, int *vector2) {
    int i;
    for (i = 0; i < TAM; ++i) {
        vector1[i] = i + 1; // The first with values as i + 1.
        vector2[i] = i + 2; // The second with values as i + 2.
    }
}

void inicializar (int *vector1, int *vector2, int *vectorT, int it, int MSGSIZE){
    int temp, pos, i;
    for(i = 0; i < MSGSIZE; ++i){
        temp = it+i;
        pos = i*2;
        if(temp < TAM){
            vectorT[pos] = vector1[temp];
            vectorT[pos + 1] = vector2[temp];
        }
        else{
            vectorT[pos] = 0;
            vectorT[pos + 1] = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    // The variables are created.
    int MSGSIZE;
    int my_rank, p, i, buf, suma, j, pos;
    MPI_Status status;
    int vector1[TAM], vector2[TAM];
    int vectorT[TAM * 2]; // vectorT is a vector that is use to pass the values through the functions.

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MSGSIZE = (int)ceil(((double)TAM / (double)(p))); // Values per process;

    if (my_rank == 0) {
        llenar(vector1, vector2);
        /*
            As the processes have a different size of the vectors, we iterate over a value probably bigger than the
            size of the vectors, if the value is bigger we use values (0) that not modifies the final sum.
        */
        for (i = MSGSIZE, j = 1; i < TAM || j < p; i += MSGSIZE, ++j) {
            inicializar(vector1, vector2, vectorT, i, MSGSIZE);
            // Broadcast to send the values to all processes.
            MPI_Send(vectorT, MSGSIZE*2, MPI_INT, j, MSGTAG, MPI_COMM_WORLD);
        }
        inicializar(vector1, vector2, vectorT, 0, MSGSIZE);
    }
    // Every process different to 0 recive the vector and then find the product of the two values gotten.
    if(my_rank != 0) {
        MPI_Recv(&vectorT, MSGSIZE*2, MPI_INT, 0, MSGTAG, MPI_COMM_WORLD, &status);
    }
    //all process do their jobs
    buf = 0;
    for(i = 0; i < MSGSIZE; ++i) {
        pos = i*2;
        buf += (vectorT[pos] * vectorT[pos+1]);
    }
    if(my_rank == 0) {
        suma = buf;
        // Broadcast to recive all the products from all processes and then add each value to suma.
        for (i = 0; i < (p-1); ++i) {
            MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MSGTAG, MPI_COMM_WORLD, &status);
            suma = suma + buf;
        }
        printf("The final value is: %d\n", suma);
    }
    else {
      printf("Envio de %d\n", my_rank);
      MPI_Send(&buf, 1, MPI_INT, 0, MSGTAG, MPI_COMM_WORLD); // Send the value to the root process.
    }

    MPI_Finalize();
    return 0;
}
