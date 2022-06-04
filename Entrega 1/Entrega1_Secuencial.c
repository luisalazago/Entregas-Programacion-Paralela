#include <stdio.h>

#define lli long long int
#define TAM 1000000

void llenar(int *vector1, int *vector2) {
    int i;
    for (i = 0; i < TAM; ++i) {
        vector1[i] = i + 1;
        vector2[i] = i + 2;
    }
}

lli calcular(int *vector1, int *vector2, int n) {
    lli suma = 0;
    int i;
    for (i = 0; i < n; ++i) {
        suma += (vector1[i] * vector2[i]);
    }
    return suma;
}

int main() {
    int vector1[TAM], vector2[TAM];
    int i;
    lli suma;
    llenar(vector1, vector2);
    calcular(vector1, vector2, TAM);
    printf("El resultado es: %lld\n", suma);
    return 0;
}