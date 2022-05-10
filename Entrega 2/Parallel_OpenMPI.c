/*
    Name: Entrega 2 Programación Paralela (OpenMPI V1).
    Date: 04/04/2022
    Authors: Guido Ernesto Salazar and Luis Alberto Salazar.
    Company: Pontificia Universidad Javeriana Cali.
*/

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

#define MSGTAG 0
#define FIL 10
#define COL 10

// We use a struct because we need to pass two values per position into the vector.
typedef struct {
    int x;
    int y;
} info;

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

void llenar_envio(int matriz[FIL][COL], int *vector, info *vectorT, int MSGSIZE, int fil, int i){
    /*
    Functions that initiallize the vector that will be used on the diferent process to
    calculate the dot product between the matrix in row_fil and the colum vector
    Inputs:
        Matrix: is a matrix of FILxCOL
        Vector: is a matrix of COLx1
		vectorT: is matrix of COLx1 
		MSGSIZE: number that indicates how much work will be used by any processe excep 0
		fil: row of the matrix
		i: point of start of the message in vector[i..i+MSGSIZE) and matrix[fil][i..i+MSGSIZE)
    Output
        vectorT: is a matrix of COLx1 with the values calculate of the vector an matrix

    */
    int k;
    for(k = 0; k < MSGSIZE; ++k){
		if(i+k < COL){
			vectorT[k].x = matriz[fil][i+k];
			vectorT[k].y = vector[i+k];
		}
		else{
			vectorT[k].x = 0;
			vectorT[k].y = 0;
		}
	}
}

int main(int argc, char *argv[]) {
    // The variables are created.
    int MSGSIZE;
    long long int suma;
    int my_rank, p, i, buf, j, k, fil;
    MPI_Status status;
    int matriz[FIL][COL], vector[COL];
    info vectorT[FIL]; // vectorT is a vector that is use to pass the values through the functions.

    MPI_Init(&argc, &argv);
    printf("Corriendo entrega con Estructura\n");
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MSGSIZE = (int)ceil(((double)COL / (p - 1))); // Values per process;

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
	
	for(fil = 0; fil < FIL; ++fil){
		buf = suma = 0;
		if (my_rank == 0) {
			// The vector is initialize
			if(fil == 0){
				llenar(matriz, vector);
			}
			/*
				As the processes have a different size of the vectors, we iterate over a value probably bigger than the
				size of the vectors, if the value is bigger we use values (0) that not modifies the vectors.
			*/
			for (i = 0, j = 0; i < MSGSIZE * (p - 1); i += MSGSIZE, ++j) {
				llenar_envio(matriz, vector, vectorT, MSGSIZE, fil, i);
				// Broadcast to send the values to all processes.
				MPI_Send(vectorT, MSGSIZE, mpi_info_type, j + 1, MSGTAG, MPI_COMM_WORLD);
			}
			// Broadcast to recive all the products from all processes and then add each value to suma.
			for (j = 0; j < (p - 1); ++j) {
				MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MSGTAG, MPI_COMM_WORLD, &status);
				suma = suma + buf;
			}
			printf("The final value is:[%d] %lld\n", fil, suma);
		}
			
		else {
			// Every process different to 0 recive the vector and then find the product of the two values gotten.
			MPI_Recv(&vectorT, MSGSIZE, mpi_info_type, 0, MSGTAG, MPI_COMM_WORLD, &status);
			for(i = 0; i < MSGSIZE; ++i) {
			buf += (vectorT[i].x * vectorT[i].y); 
			}
			MPI_Send(&buf, 1, MPI_INT, 0, MSGTAG, MPI_COMM_WORLD); // Send the value to the root process.
		}
	}

    MPI_Type_free(&mpi_info_type);
    MPI_Finalize();
    return 0;
}
