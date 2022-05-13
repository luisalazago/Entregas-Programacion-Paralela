/*******************************************************************************
*
*  MatMatMult1.c -  A program to multiply two matrices using nested loops
*
*   Notes:            Matrices are wired and have integer elements
*                     Uses pointers to the matrices and pointer arithmetic
*                     Uses a straightforward approach
*
*******************************************************************************/

#include <stdio.h>

#define   N         16
#define   MAT_ROWS  N
#define   MAT_COLS  N

int matA [N][N],
    matB [N][N],
    matY [N][N] = { {0} };

/*--( Matrix Matrix Multiplication )-------------------------------------*/

void MatrixMatrixMult ( int *MA, int *MB, int *MY, int size )
{
  for ( int row = 0; row < size; row++ )
    for ( int col = 0; col < size; col++ )
    {
      int sum = 0;

      for ( int i = 0; i < size; i++ )
      {
        int a,
            b;

        a = MA [row * size + i];
        b = MB [i * size + col];
        sum += a * b;
      }
      MY [row * size + col] = sum;
    }
}

/*--( Support functions )------------------------------------------------*/

/* Initialise matrices */
void init_matrices ( int *MA, int *MB )
{
  int row,
      col;

  for ( row = 0; row < N; row++ )
    for ( col = 0; col < N; col++ )
    {
      MA[row * N + col] = row * MAT_COLS + col;
      MB[row * N + col] = row * MAT_COLS + col + MAT_ROWS * MAT_COLS;
    }
}

/* Print matrix */
void PrintMatrix ( int *matrix )
{
  int row,
      col;

  for ( row = 0; row < N; row++ )
  {
    for ( col = 0; col < N; col++ )
      printf ( "%7d ", matrix [row * N + col] );
    printf ( "\n" );
  }
  printf ( "\n" );
}

/*--( Main function )----------------------------------------------------*/

int main ( void )
{
  int size = N;
  init_matrices ( (int *) matA, (int *) matB );

  printf ( "\n16x16 A Matrix is \n" );
  PrintMatrix ( (int *) matA );

  printf ( "\n16x16 B Matrix is \n" );
  PrintMatrix ( (int *) matB );

  MatrixMatrixMult ( (int *) matA, (int *) matB, (int *) matY, size );

  printf ( "\nResulting 16x16 Y Matrix is \n" );
  PrintMatrix ( (int *) matY );

  return ( 0 );
}
