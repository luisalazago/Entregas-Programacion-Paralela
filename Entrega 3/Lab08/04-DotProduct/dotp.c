/*******************************************************************************
*                                                                              *
*  dotp.c -                                                                    *
*                                                                              *
*   Dot product on the host (CPU)                                              *
*                                                                              *
*                     Departamento de Electronica y Ciencias de la Computacion *
*                     Pontificia Universidad Javeriana - CALI                  *
*                                                                              *
*******************************************************************************/

#include    <stdio.h>
#include    <stdlib.h>
#include    <stdbool.h>

/***( Manifest Constants )************************************************/

#define N   512

/***( Program Code )******************************************************/

/*--( Support functions )------------------------------------------------*/

/* Dot product of length N vectors */
void dotp ( int *v1, int *v2, int *result )
{
  int   i;

  for ( i = 0; i < N; i++ )
    *result += v1 [i] * v2 [i];
}

/* Initialise vectors of the given length */
void init_vectors ( int *v1, int *v2, int length )
{
  int i;

  for ( i = 0; i < length; i++ )
  {
    v1[i] = i;
    v2[i] = 2 * i;
  }
}

/*--( Main function )----------------------------------------------------*/

int main ( void )
{
  int  *v1, *v2;
  int  result = 0;
  int     size = N * sizeof (int);

  /* Allocate space for copies of v1, v2; setup input values */
  v1 = (int *) malloc ( size );
  v2 = (int *) malloc ( size );
  init_vectors ( v1, v2, N );

  /* Call dotp () */
  dotp ( v1, v2, &result );

  /* verify that the calculation is correct */
  bool success = true;
  #define sum_squares(x)  (int) ( (x) * ( (x) + 1 ) * ( 2 * (x) + 1 ) / 6 )
  if ( result != 2 * sum_squares ( N - 1 ) )
    success = false;
  if ( success )
    printf ( "Sequential dot product (%d) matches golden ref (%d)\n", result, 2 * sum_squares ( N - 1 ) );

  /* Cleanup */
  free ( v1 );
  free ( v2 );

  return ( 0 );
}
