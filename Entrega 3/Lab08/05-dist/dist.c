/*******************************************************************************
*                                                                              *
*  dist.c - Compute an array of distances from a reference point to each of    *
*           N points uniformly spaced along a line segment                     *
*                     Departamento de Electronica y Ciencias de la Computacion *
*                     Pontificia Universidad Javeriana - CALI                  *
*                                                                              *
*******************************************************************************/

#include <math.h>
#include <stdio.h>

#define N 64

/***( support functions )*************************************************/

float scale ( int i, int n )
{
  return ( ( (float) i ) / ( n - 1 ) );
}

float distance ( float x1, float x2 )
{
  return ( sqrt ( (x2 - x1) * (x2 - x1) ) );
}

/***( main function )*****************************************************/

int main ( void )
{
  float out[N] = { 0.0 };
  const float ref = 0.5;

  for ( int i = 0; i < N; ++i )
  {
    float x = scale ( i, N );
    out[i] = distance ( x, ref );
    printf ( "i = %2d: dist from %f to %f is %f.\n", i, ref, x, out[i] );
  }

  return ( 0 );
}
