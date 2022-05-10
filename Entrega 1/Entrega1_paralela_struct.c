/*
    Name: Entrega 1 Programación Paralela (Struct Version).
    Date: 02/03/2022
    Authors: Guido Ernesto Salazar and Luis Alberto Salazar.
    Company: Pontificia Universidad Javeriana Cali.
*/

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

#define MSGTAG 0
#define TAM 1000000

// We use a struct because we need to pass two values per position into the vector.
typedef struct {
    int x;
    int y;
} info;

int main(int argc, char *argv[]) {
    // The variables are created.
    int MSGSIZE;
    long long int suma;
    int my_rank, p, i, buf, j, k;
    MPI_Status status;
    int vector1[TAM], vector2[TAM];
    info vectorT[TAM]; // vectorT is a vector that is use to pass the values through the functions.

    MPI_Init(&argc, &argv);
    printf("Corriendo entrega con Estructura\n");
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    buf = suma = 0;
    MSGSIZE = (int)ceil(((double)TAM / (p - 1))); // Values per process;

    // Initialize mpi_info datatype for sharing memory
    const int nitems=2;
    int          blocklengths[2] = {1,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Datatype mpi_info_type;
    MPI_Aint     offsets[2];

    offsets[0] = offsetof(info, x);
    offsets[1] = offsetof(info, y);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_info_type);
    MPI_Type_commit(&mpi_info_type);

    if (my_rank == 0) {
        // The vector is initialize
        for (i = 0; i < TAM; ++i) {
            vector1[i] = i + 1; // The first with values as i + 1.
            vector2[i] = i + 2; // The second with values as i + 2.
        }


        /*
            As the processes have a different size of the vectors, we iterate over a value probably bigger than the
            size of the vectors, if the value is bigger we use values (0) that not modifies the vectors.
        */
        for (i = 0, j = 0; i < MSGSIZE * (p - 1); i += MSGSIZE, ++j) {
            for(k = 0; k < MSGSIZE; ++k){
                if(i+k < TAM){
                    vectorT[k].x = vector1[i+k];
                    vectorT[k].y = vector2[i+k];
                }
                else{
                    vectorT[k].x = 0;
                    vectorT[k].y = 0;
                }
            }
            // printf("++ %d --%d\n", MSGSIZE * (p - 1), i);
            
            // Broadcast to send the values to all processes.
            // printf("** %d -- %d\n", j+1, i);
            MPI_Send(vectorT, MSGSIZE, mpi_info_type, j + 1, MSGTAG, MPI_COMM_WORLD);
        }

        // Broadcast to recive all the products from all processes and then add each value to suma.
        for (j = 0; j < (p - 1); ++j) {
            MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MSGTAG, MPI_COMM_WORLD, &status);
            suma = suma + buf;
        }
        printf("The final value is: %lld\n", suma);
    }
    else {
        // Every process different to 0 recive the vector and then find the product of the two values gotten.
        MPI_Recv(&vectorT, MSGSIZE, mpi_info_type, 0, MSGTAG, MPI_COMM_WORLD, &status);
        for(i = 0; i < MSGSIZE; ++i) {
           buf += (vectorT[i].x * vectorT[i].y); 
        }
        MPI_Send(&buf, 1, MPI_INT, 0, MSGTAG, MPI_COMM_WORLD); // Send the value to the root process.
    }

    MPI_Type_free(&mpi_info_type);
    MPI_Finalize();
    return 0;
}
