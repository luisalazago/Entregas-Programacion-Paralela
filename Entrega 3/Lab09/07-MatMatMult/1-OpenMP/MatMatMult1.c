/*******************************************************************************
*
*  MatMatMult1.c -    A program to multiply two matrices using OpenMP
*
*   Notes:            Matrices are wired and have integer elements
*                     Uses a straightforward approach and nested loops
*
*******************************************************************************/

#include <stdio.h>
#include <omp.h>

#define   N         16
#define   MAT_ROWS  N
#define   MAT_COLS  N

int matA [N][N],
    matB [N][N],
    matY [N][N] = { {0} };

/*--( Matrix Matrix Multiplication )-------------------------------------*/

void MatrixMatrixMult ( int MA[MAT_ROWS][MAT_COLS], int MB[MAT_ROWS][MAT_COLS], int MY[MAT_ROWS][MAT_COLS], int size )
{
  int row,
      col,
      i;

  #pragma omp parallel private(row,col,i)
  {
    #pragma omp for
      for ( row = 0; row < size; row++ )
      {
          #pragma omp parallel shared(MA,MB,MY,size)
          {
            #pragma omp for
            for ( col = 0; col <size; col++ )
            {
              MY[row][col] = 0;
              for ( i = 0; i < size; i++ )
                MY[row][col] += MA[row][i] * MB[i][col];
            }
          }
      }
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

  MatrixMatrixMult ( matA, matB, matY, size );

  printf ( "\nResulting 16x16 Y Matrix is \n" );
  PrintMatrix ( (int *) matY );

  return ( 0 );
}
