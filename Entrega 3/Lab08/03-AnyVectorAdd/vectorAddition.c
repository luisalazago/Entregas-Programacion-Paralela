/*******************************************************************************
*                                                                              *
*  vectorAddition.c -                                                          *
*                                                                              *
*   Adds two vectors on the host (CPU)                                         *
*                                                                              *
*                     Departamento de Electronica y Ciencias de la Computacion *
*                     Pontificia Universidad Javeriana - CALI                  *
*                                                                              *
*******************************************************************************/

#include    <stdio.h>
#include    <stdlib.h>
#include    <stdbool.h>

/***( Manifest Constants )************************************************/

#define N   16 * 1024 * 1024

/***( Program Code )******************************************************/

/*--( Support functions )------------------------------------------------*/

/* Add two vectors of length N */
void add ( int *augend, int *addend, int *result )
{
  int i;

  for ( i = 0; i < N; i++ )
    result [i] = augend [i] + addend [i];
}

/* Initialise a vector of the given length */
void init_vect ( int *vector, int length )
{
  int i;

  for ( i = 0; i < length; i++ )
    vector[i] = i;
}

/*--( Main function )----------------------------------------------------*/

int main ( void )
{
  int *augend, *addend, *result;
  int size = N * sizeof (int);
  int i;

  /* Allocate space for copies of augend, addend, result; setup input values */
  augend = (int *) malloc ( size ); init_vect ( augend, N );
  addend = (int *) malloc ( size ); init_vect ( addend, N );
  result = (int *) malloc ( size );

  /* Call add () */
  add ( augend, addend, result );

  /* verify that the calculation is correct */
  bool success = true;

  for ( i = 0; i < N; i++ )
  {
    if ( result[i] != ( 2 * i ) )
    {
      printf ( "Aaaargh! Result at element %d (%d) doesn't match golden ref (%d)!\n",
               i, result[i], 2 * i );
      success = false;
    }
  }
  if ( success )
    printf ( "Sequential vector addition of %d elements matches golden ref\n", N );

  /* Cleanup */
  free ( augend );
  free ( addend );
  free ( result );

  return ( 0 );
}
