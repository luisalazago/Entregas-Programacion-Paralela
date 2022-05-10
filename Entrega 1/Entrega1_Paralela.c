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

int main(int argc, char *argv[]) {
    // The variables are created.
    int MSGSIZE;
    int my_rank, p, i, buf, suma, j, k;
    MPI_Status status;
    int vector1[TAM], vector2[TAM];
    int vectorT[TAM * 2]; // vectorT is a vector that is use to pass the values through the functions.

    MPI_Init(&argc, &argv);
    printf("Corriendo entrega sin Estructura\n");
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    buf = suma = 0;
    MSGSIZE = (int)ceil(((double)TAM / (p - 1))); // Values per process;

    if (my_rank == 0) {
        for (i = 0; i < TAM; ++i) {
            vector1[i] = i + 1; // The first with values as i + 1.
            vector2[i] = i + 2; // The second with values as i + 2.
        }
        /*
            As the processes have a different size of the vectors, we iterate over a value probably bigger than the
            size of the vectors, if the value is bigger we use values (0) that not modifies the final sum.
        */
        for (i = 0, j = 0; i < MSGSIZE * (p - 1); i += MSGSIZE, ++j) {
            for(k = 0; k < MSGSIZE; ++k){
                if(i+k < TAM){
                    vectorT[k*2] = vector1[i+k];
                    vectorT[(k*2) + 1] = vector2[i+k];
                }
                else{
                    vectorT[k*2] = 0;
                    vectorT[(k*2) + 1] = 0;
                }
            }
            // Broadcast to send the values to all processes.
            MPI_Send(vectorT, MSGSIZE*2, MPI_INT, j + 1, MSGTAG, MPI_COMM_WORLD);
        }

        // Broadcast to recive all the products from all processes and then add each value to suma.
        for (j = 0; j < (p - 1); ++j) {
            MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MSGTAG, MPI_COMM_WORLD, &status);
            suma = suma + buf;
        }
        printf("The final value is: %d\n", suma);
    }
    else {
        // Every process different to 0 recive the vector and then find the product of the two values gotten.
        MPI_Recv(&vectorT, MSGSIZE*2, MPI_INT, 0, MSGTAG, MPI_COMM_WORLD, &status);
        for(i = 0; i < MSGSIZE; ++i) {
           buf += (vectorT[i*2] * vectorT[(i*2)+1]); 
        }
        MPI_Send(&buf, 1, MPI_INT, 0, MSGTAG, MPI_COMM_WORLD); // Send the value to the root process.
    }

    MPI_Finalize();
    return 0;
}
