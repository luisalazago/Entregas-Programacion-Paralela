/*******************************************************************************
*
*  MatMatMult6.c -  A program to multiply two matrices using nested loops
*
*   Notes:            Matrices are dynamically allocated for double elements
*                     Uses pointers, ptr arith, and a straightforward approach
*
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "timer.h"

/*--( Matrix Matrix Multiplication )-------------------------------------*/

void MatrixMatrixMult ( double *MA, double *MB, double *MY, int size )
{
  int row,
      col,
      i;

  for ( row = 0; row < size; row++ )
    for ( col = 0; col < size; col++ )
      for ( i = 0; i < size; i++ )
      {
        double  a,
                b;

        a = MA [row * size + i];
        b = MB [i * size + col];
        MY [row * size + col] += a * b;
      }
}

/*--( Support functions )------------------------------------------------*/

/* Initialise matrices */
void init_matrices ( double *MA, double *MB, double *MY, int size )
{
  int row,
      col;

  for ( row = 0; row < size; row++ )
    for ( col = 0; col < size; col++ )
    {
      MA[row * size + col] = sin ( row * size + col );
      MB[row * size + col] = cos ( row * size + col + size * size );
      MY[row * size + col] = 0;
    }
}

/* Print matrix */
void PrintMatrix ( double *matrix, int size )
{
  int row,
      col;

  for ( row = 0; row < size; row++ )
  {
    for ( col = 0; col < size; col++ )
      printf ( "%.6f ", matrix [row * size + col] );
    printf ( "\n" );
  }
  printf ( "\n" );
}

/*--( Main function )----------------------------------------------------*/

#define NTIMES 16

int main ( int argc, char* argv[] )
{
  double  *matA,
          *matB,
          *matY;

  struct timespec tstart;
  double time_sum = 0.0;

  if ( argc != 2 )
  {
    printf ( "Usage: %s N; where N is the number of rows and cols in the test matrices\n", argv[0] );
    return ( -1 );
  }

  int N = atoi ( argv[1] );
  int M = N * 32;
  int size = M * M * sizeof(double);      /* ( N x 32 ) x ( N x 32 ) = N x N Ki elements */

  matA = (double *) malloc ( size );
  matB = (double *) malloc ( size );
  matY = (double *) malloc ( size );

  init_matrices ( matA, matB, matY, M );

  for ( int k = 0; k < NTIMES; k++ )
  {
    cpu_timer_start ( &tstart );

    MatrixMatrixMult ( matA, matB, matY, M );

    time_sum += cpu_timer_stop ( tstart );
   }

  free ( matA );
  free ( matB );
  free ( matY );

  printf ( "%s: Average runtime using %dx%d Ki matrix sizes is %lf msecs\n", argv[0], N, N, time_sum / NTIMES );

  return ( 0 );
}
