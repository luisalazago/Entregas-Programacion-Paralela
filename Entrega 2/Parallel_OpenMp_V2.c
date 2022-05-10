/*
    Name: Entrega 2 Programación Paralela (OpenMP V2).
    Date: 04/04/2022
    Authors: Guido Ernesto Salazar and Luis Alberto Salazar.
    Company: Pontificia Universidad Javeriana Cali.
*/

#include <stdio.h>
#include <omp.h>

#define TAM 10 // Size of the rows of the matrix.
#define COL 10 // Size of the vector and the columns of the matrix.

void fill(int matrix[TAM][COL], int vector[COL]) {
	int i, j;
	// Fill the vector and matrix.
	for(j = 0; j < COL; ++j) {
		/*
			We filling first the columns because the vector is filling one time, if we would fill
			the rows first the vector would be fill TAM times.
		*/
		for(i = 0; i < TAM; ++i) {
			matrix[i][j] = i + j;
		}
		vector[j] = j + 1;
	}
}

int main(int argc, char* argv[]) {
	int i, j;
	int vector[COL], ans[TAM];
	int matrix[TAM][COL];
	int suma;

	fill(matrix, vector);

	/*
		In this part we use the pragma in the highest loop because the complexity is less than use the pragma
		in the deepest loop and if we use the pragma in the columns the data is not consecutive so it will be
		many misses.
	*/
	#pragma omp parallel for private(i, j) reduction(+:suma)
	for(i = 0; i < TAM; ++i) {
		suma = 0;
		for(j = 0; j < COL; ++j) {
			suma += matrix[i][j] * vector[j];
		}
		ans[i] = suma;
	}

	/*
		The result is a vector of rows with one column that will have the scalar product of
		each element of the matrix and each element of the vector.
	*/
	printf("El resultado del vector es: \n");
	for(i = 0; i < TAM; ++i) {
		printf("[%d]: %d\n", i, ans[i]);
	}

	return 0;
}