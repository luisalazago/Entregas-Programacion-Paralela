/*******************************************************************************
*
*  MatMatMult2.c -  A program to multiply two matrices using nested loops
*
*   Notes:            Matrices are wired and have integer elements
*                     Uses a straightforward approach
*                     Swaps inner loops
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

void MatrixMatrixMult ( int MA[][N], int MB[][N], int MY[][N], int size )
{
  int row,
      col,
      i;

  for ( row = 0; row < size; row++ )
    for ( i = 0; i < size; i++ )
      for ( col = 0; col < size; col++ )
        MY [row][col] += MA [row][i] * MB [i][col];
}

/*--( Support functions )------------------------------------------------*/

/* Initialise matrices */
void init_matrices ( int MA[][N], int MB[][N] )
{
  int row,
      col;

  for ( row = 0; row < N; row++ )
    for ( col = 0; col < N; col++ )
    {
      MA[row][col] = row * MAT_COLS + col;
      MB[row][col] = row * MAT_COLS + col + MAT_ROWS * MAT_COLS;
    }
}

/* Print matrix */
void PrintMatrix ( int matrix [][N] )
{
  int row,
      col;

  for ( row = 0; row < N; row++ )
  {
    for ( col = 0; col < N; col++ )
      printf ( "%7d ", matrix [row][col] );
    printf ( "\n" );
  }
  printf ( "\n" );
}

/*--( Main function )----------------------------------------------------*/

int main ( void )
{
  int size = N;
  init_matrices ( matA, matB );

  printf ( "\n16x16 A Matrix is \n" );
  PrintMatrix ( matA );

  printf ( "\n16x16 B Matrix is \n" );
  PrintMatrix ( matB );

  MatrixMatrixMult ( matA, matB, matY, size );

  printf ( "\nResulting 16x16 Y Matrix is \n" );
  PrintMatrix ( matY );

  return ( 0 );
}
