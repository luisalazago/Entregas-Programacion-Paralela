/*******************************************************************************
*
*  MatMatMult8.c -  A program to multiply two matrices using nested loops
*                   and slicing the matrices into tiles
*
*   Notes:            Matrices are dynamically allocated for int elements
*                     using a 2D notation
*                     Uses pointers, ptr arith, and a straightforward approach
*
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*--( Submatrix Submatrix Multiplication )-------------------------------*/

void BlkBlkMult ( int **bMA, int **bMB, int **bMY, int size, int bsize, int row, int col, int i )
{
  int brow,
      bcol,
      bi;

  for ( brow = 0; brow < bsize; brow++ )
    for ( bcol = 0; bcol < bsize; bcol++ )
      for ( bi = 0; bi < bsize; bi++ )
      {
        int  a,
             b;

        a = bMA [row + brow][i + bi];
        b = bMB [i + bi][col + bcol];
        bMY [row + brow][col + bcol] += a * b;
      }
}

/*--( Matrix Matrix Multiplication )-------------------------------------*/

void MatrixMatrixMult ( int **MA, int **MB, int **MY, int size, int bsize )
{
  int row,
      col,
      i;

  for ( row = 0; row < size; row += bsize )
    for ( col = 0; col < size; col += bsize )
      for ( i = 0; i < size; i += bsize )
        BlkBlkMult ( MA, MB, MY, size, bsize, row, col, i );
}

/*--( Support functions )------------------------------------------------*/

/* Initialise matrices */
void init_matrices ( int **MA, int **MB, int **MY, int size )
{
  int row,
      col;

  for ( row = 0; row < size; row++ )
    for ( col = 0; col < size; col++ )
    {
      MA[row][col] = 1;
      MB[row][col] = 2;
      MY[row][col] = 0;
    }
}

/* Print matrix */
void PrintMatrix ( int **matrix, int size )
{
  int row,
      col;

  for ( row = 0; row < size; row++ )
  {
    for ( col = 0; col < size; col++ )
      printf ( "%3d ", matrix [row][col] );
    printf ( "\n" );
  }
  printf ( "\n" );
}

/*--( Main function )----------------------------------------------------*/

int main ( int argc, char* argv[] )
{
  int **matA,
      **matB,
      **matY;

  int i;

  if ( argc != 3 )
  {
    printf ( "Usage: %s N B; where N is the number of rows and cols in the test matrices and\n\t\t\t\tB is the number of rows and cols in the block matrices\n", argv[0] );
    return ( -1 );
  }

  int N = atoi ( argv[1] );
  int size = N * N * sizeof(int);

  int B = atoi ( argv[2] );

  matA = (int **) malloc ( N * sizeof(int *) );
  matA[0] = (int *) malloc ( size );
  for ( i = 1; i < N; i++ )
    matA[i] = matA[0] + i * N;

  matB = (int **) malloc ( N * sizeof(int *) );
  matB[0] = (int *) malloc ( size );
  for ( i = 1; i < N; i++ )
    matB[i] = matB[0] + i * N;

  matY = (int **) malloc ( N * sizeof(int *) );
  matY[0] = (int *) malloc ( size );
  for ( i = 1; i < N; i++ )
    matY[i] = matY[0] + i * N;

  init_matrices ( matA, matB, matY, N );

  if ( N <= 32 )
  {
    printf ( "\n%dx%d A Matrix is \n", N, N );
    fflush ( stdout );
    PrintMatrix ( matA, N );

    printf ( "\n%dx%d B Matrix is \n", N, N );
    PrintMatrix ( matB, N );
  }

  MatrixMatrixMult ( matA, matB, matY, N, B );

  if ( N <= 32 )
  {
    printf ( "\nResulting %dx%d Y Matrix is \n", N, N );
    PrintMatrix ( matY, N );
  }

  free ( matA[0] );
  free ( matA );

  free ( matB[0] );
  free ( matB );

  free ( matY[0] );
  free ( matY );

  return ( 0 );
}
