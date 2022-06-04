/*
    Name: Entrega 1 Programación Paralela (Binomial Version).
    Date: 02/03/2022
    Authors: Guido Ernesto Salazar and Luis Alberto Salazar.
    Company: Pontificia Universidad Javeriana Cali.
*/

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

#define MSGTAG 0
#define TAM 16

// We use a struct because we need to pass two values per position into the vector.
typedef struct {
    int x;
    int y;
} info;

void llenar(info *vectorT) {
    int i;
    for (i = 0; i < TAM; ++i) {
        vectorT[i].x = i + 1; // The first with values as i + 1.
        vectorT[i].y = i + 2; // The second with values as i + 2.
    }
}

int main(int argc, char *argv[]) {
    // The variables are created.
    int MSGSIZE, tam, cant;
    int my_rank, p, i, buf, suma, j;
    MPI_Status status;
    info vectorT[TAM]; // vectorT is a vector that is use to pass the values through the functions.

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    buf = suma = 0;
    tam = ceil(log2(p)); // Values per process;
    MSGSIZE = (int)ceil(((double)TAM / (double)(p))); // Values per process;

    // Initialize mpi_info datatype for sharing memory
    const int    nitems = 2;
    int          blocklengths[2] = {1,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Datatype mpi_info_type;
    MPI_Aint     offsets[2];

    offsets[0] = offsetof(info, x);
    offsets[1] = offsetof(info, y);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_info_type);
    MPI_Type_commit(&mpi_info_type);

    if (my_rank == 0) {
        printf("Corriendo entrega con Estructura\n");
        llenar(vectorT);
        /*
            As the processes have a different size of the vectors, we iterate over a value probably bigger than the
            size of the vectors, if the value is bigger we use values (0) that not modifies the final sum.
        */
        cant = TAM;
        for (i = 0; i < tam; ++i) {
            cant /= 2;
            // Broadcast to send the values to all processes.
            if(my_rank+pow(2, i) < p)
              MPI_Send((vectorT+cant), cant, mpi_info_type, my_rank+pow(2, i), MSGTAG, MPI_COMM_WORLD);
        }
    }
    // Every process different to 0 recive the vector and then find the product of the two values gotten.
    else{
        j = (int)floor(log2(my_rank));
        cant = TAM/(pow(2,j+1));
        MPI_Recv(&vectorT, cant, mpi_info_type,  my_rank-pow(2, j), MSGTAG, MPI_COMM_WORLD, &status);
        for(i=j+1; i < tam; ++i){
            cant /= 2;
            if(my_rank+pow(2,i) < p)
              MPI_Send((vectorT+cant), cant, mpi_info_type, my_rank+pow(2, i), MSGTAG, MPI_COMM_WORLD);
        }
    }
    //all process do their jobs
    buf = 0;
    for(i = 0; i < MSGSIZE; ++i) {
        buf += (vectorT[i].x * vectorT[i].y);
    }
    printf("%d %d\n", my_rank, buf);
    if(my_rank == 0) {
        // Broadcast to recive all the products from all processes and then add each value to suma.
        for (i = 0; i < tam; ++i) {
            MPI_Recv(&suma, 1, MPI_INT, MPI_ANY_SOURCE, MSGTAG, MPI_COMM_WORLD, &status);
            buf += suma;
        }
        printf("The final value is: %d\n", buf);
    }
    else {
        j = (int)floor(log2(my_rank));
        for(i=j+1; i < tam; ++i){
            MPI_Recv(&suma, 1, MPI_INT, MPI_ANY_SOURCE, MSGTAG, MPI_COMM_WORLD, &status);
            buf += suma;
        }
        MPI_Send(&buf, 1, MPI_INT, my_rank-pow(2, j), MSGTAG, MPI_COMM_WORLD);
    }

    MPI_Type_free(&mpi_info_type);
    MPI_Finalize();
    return 0;
}
