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

// We use a struct because we need to pass two values per position into the vector.
typedef struct {
    int x;
    int y;
} info;

int main(int argc, char *argv[]) {
    // The variables are created.
    int MSGSIZE, TAM = 10;
    int my_rank, p, i, buf, suma, j, k;
    MPI_Status status;
    int vector1[TAM], vector2[TAM];
    info vectorT[TAM]; // vectorT is a vector that is use to pass the values through the functions.

    MPI_Init(&argc, &argv);
    printf("Corriendo entrega con Estructura\n");
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    buf = suma = 0;
    MSGSIZE = ceil(log2(p)); // Values per process;

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
        // The vector is initialize
        for (i = 0; i < TAM; ++i) {
            vector1[i] = i + 1; // The first with values as i + 1.
            vector2[i] = i + 2; // The second with values as i + 2.
        }

        /*
            As the processes have a different size of the vectors, we iterate over a value probably bigger than the
            size of the vectors, if the value is bigger we use values (0) that not modifies the vectors.
        */
        for (i = 0; i < MSGSIZE; ++i) {
            for(k = TAM / 2; k < TAM; ++k){
                vectorT[k].x = vector1[k];
				vectorT[k].y = vector2[k];
            }

            // Binomial way to send the information to the next 2^i node.
            TAM = TAM / 2;
            MPI_Send(vectorT, TAM, mpi_info_type, pow(2, i), MSGTAG, MPI_COMM_WORLD);
        }

        // Broadcast to recive all the products from all processes and then add each value to suma.
        suma = (vectorT[0].x * vectorT[0].y);
        for (i = 0; i < MSGSIZE; ++i) {
            MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MSGTAG, MPI_COMM_WORLD, &status);
            suma = suma + buf;
        }
        printf("The final value is: %d\n", suma);
    }
    else {
    	if(TAM > 1) {
    		// Every process different to 0 recive the vector and then find the product of the two values gotten.
	        MPI_Recv(&vectorT, TAM, mpi_info_type, MPI_ANY_SOURCE, MSGTAG, MPI_COMM_WORLD, &status);
	        TAM = TAM / 2;
	        MPI_Send(vectorT, TAM, mpi_info_type, , MSGTAG, MPI_COMM_WORLD); // Send the value to the root process.
    	}
    	/*
    		We have to finish this part and build the MPI_Recv and MPI_Send to calculate and send the information
    		when we are on the last level and process.
    	*/
    }

    MPI_Type_free(&mpi_info_type);
    MPI_Finalize();
    return 0;
}
