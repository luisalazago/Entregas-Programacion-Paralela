/*
    Name: Entrega 2 Programación Paralela (Sequential Version).
    Date: 04/04/2022
    Authors: Guido Ernesto Salazar and Luis Alberto Salazar.
    Company: Pontificia Universidad Javeriana Cali.
*/

#include <stdio.h>

#define FIL 10
#define COL 10

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

void productoPunto(int matrix[FIL][COL], int *vector, int *ans){
    /*
    Functions that calculate the dot product between a matrix and a vector
    Inputs:
        Matrix: is a matrix of FILxCOL
        Vector: is a matrix of COLx1
    Outputs
        ans: is a matrix of FILx1 where each position has the value of the dot
        product between row_i and the vector 

    */
    int i, j, suma;
    for(i = 0; i < FIL; ++i){
        suma = 0;
        for(j = 0; j < COL; ++j)
            suma += matrix[i][j] * vector[j];
        ans[i] = suma;
    }
}

int main(){
    int matrix[FIL][COL], vector[COL];
    int ans[FIL];
    int i, j;
    int suma;
    llenar(matrix, vector);
    productoPunto(matrix, vector, ans);
    printf("El resultado del producto punto es un vector con los valores: \n");
    for(i = 0; i < COL; ++i)
        printf("[%d] = %d\n", i, ans[i]);
    printf("\n");
    return 0;
}