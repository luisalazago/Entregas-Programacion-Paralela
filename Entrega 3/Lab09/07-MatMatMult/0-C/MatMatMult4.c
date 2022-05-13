/*******************************************************************************
*
*  MatMatMult4.c -  A program to multiply two matrices using nested loops
*
*   Notes:            Matrices are dynamically allocated for double elements
*                     Uses pointers, ptr arith, and a straightforward approach
*
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

int main ( int argc, char* argv[] )
{
  double  *matA,
          *matB,
          *matY;

  if ( argc != 2 )
  {
    printf ( "Usage: %s N; where N is the number of rows and cols in the test matrices\n", argv[0] );
    return ( -1 );
  }

  int N = atoi ( argv[1] );
  int size = N * N * sizeof(double);

  matA = (double *) malloc ( size );
  matB = (double *) malloc ( size );
  matY = (double *) malloc ( size );

  init_matrices ( matA, matB, matY, N );

  if ( N <= 32 )
  {
    printf ( "\n%dx%d A Matrix is \n", N, N );
    PrintMatrix ( matA, N );

    printf ( "\n%dx%d B Matrix is \n", N, N );
    PrintMatrix ( matB, N );
  }

  MatrixMatrixMult ( matA, matB, matY, N );

  if ( N <= 32 )
  {
    printf ( "\nResulting %dx%d Y Matrix is \n", N, N );
    PrintMatrix ( matY, N );
  }

  free ( matA );
  free ( matB );
  free ( matY );

  return ( 0 );
}
