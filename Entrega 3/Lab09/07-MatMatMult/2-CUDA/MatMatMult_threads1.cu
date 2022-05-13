/*******************************************************************************
*
*  MatMatMult_threads1.cu -  A program to multiply two matrices using CUDA
*
*   Notes:            Matrices are wired and have integer elements
*                     Uses pointers to the matrices and pointer arithmetic
*                     Uses global memory; each thread computes one element
*                     of the block sub-matrix
*                     Prevent in excess threads to perform out-of-bounds
*                     operations
*
*******************************************************************************/

#include <stdio.h>

#define N           16
#define MAT_COLS    N
#define MAT_ROWS    N

#define MAX_THREADS 512

/***( CUDA kernel )*******************************************************/

__global__ void MatMulKernel ( int * MA, int * MB, int * MY )
{
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;

  int Yval = 0;

  /* prevent in excess threads to perform out-of-bounds operations */
  if ( row < N && col < N )
  {
    for ( int i = 0; i < N; i++ )
      Yval += MA[row * N + i] * MB[i * N + col];
  }
  MY[row * N + col] = Yval;
}

/*--( Support functions )------------------------------------------------*/

/* Initialise matrices of the given size */
void init_matrices ( int *MA, int *MB, int size )
{
  int row,
      col;

  for ( row = 0; row < size; row++ )
    for ( col = 0; col < size; col++ )
    {
      MA[row * N + col] = row * MAT_COLS + col;
      MB[row * N + col] = row * MAT_COLS + col + MAT_ROWS * MAT_COLS;
    }
}

/* Print matrix of the given size */
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
  int *MA,
      *MB,
      *MY;
  int *d_MA,
      *d_MB,
      *d_MY;

  int size = N * N * sizeof(int);

  MA = (int *) malloc ( size );
  MB = (int *) malloc ( size );
  MY = (int *) malloc ( size );

  /* Initialise A, B matrices */
  init_matrices ( MA, MB, N );

  printf ( "\n16x16 A matrix is \n" );
  PrintMatrix ( MA );

  printf ( "\n16x16 B matrix is \n" );
  PrintMatrix ( MB );

  /* set MY = { {0} } */
  MY = (int *) malloc ( size );
  memset ( (void *) MY, 0, size );

  cudaMalloc ( &d_MA, size );
  cudaMalloc ( &d_MB, size );

  cudaMemcpy ( d_MA, MA, size, cudaMemcpyHostToDevice );
  cudaMemcpy ( d_MB, MB, size, cudaMemcpyHostToDevice );

  cudaMalloc ( &d_MY, size );
  cudaMemcpy ( d_MY, MY, size, cudaMemcpyHostToDevice );

  dim3 BlocksPerGrid ( 1, 1 );
  dim3 ThreadsPerBlock ( N, N );
  if ( N * N > MAX_THREADS )
  {
    ThreadsPerBlock.x = MAX_THREADS;
    ThreadsPerBlock.y = MAX_THREADS;
    BlocksPerGrid.x = ceil ( (double) N / (double) ThreadsPerBlock.x );
    BlocksPerGrid.y = ceil ( (double) N / (double) ThreadsPerBlock.y );
  }

  MatMulKernel <<< BlocksPerGrid, ThreadsPerBlock >>> ( d_MA, d_MB, d_MY );

  cudaMemcpy ( MY, d_MY, size, cudaMemcpyDeviceToHost );

  printf ( "\n16x16 Y matrix is \n" );
  PrintMatrix ( MY );

  cudaFree ( d_MA );
  cudaFree ( d_MB );
  cudaFree ( d_MY );

  free ( MA );
  free ( MB );
  free ( MY );
}
