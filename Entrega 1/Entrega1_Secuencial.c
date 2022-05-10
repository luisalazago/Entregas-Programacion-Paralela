#include <stdio.h>

#define TAM 1000000

int main()
{
    int vector1[TAM], vector2[TAM];
    int i;
    long long int suma;

    for (i = 0; i < TAM; ++i)
    {
        vector1[i] = i + 1;
        vector2[i] = i + 2;
    }

    suma = 0;
    for (i = 0; i < TAM; ++i)
    {
        suma = suma + (vector1[i] * vector2[i]);
    }

    printf("El resultado es: %lld\n", suma);
    return 0;
}