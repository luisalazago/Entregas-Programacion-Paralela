/*******************************************************************************
*
*  MatMatMult_blocks_threads.cu -  A program to multiply two matrices using CUDA
*
*   Notes:            Matrices are wired and have integer elements
*                     Uses pointers to the matrices and pointer arithmetic
*                     Uses global memory; each thread computes one element
*                     of the block sub-matrix
*                     Prevent in excess threads to perform out-of-bounds
*                     operations
*                     Matrix size and GPU blocks are arguments in the command
*                     line
*
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>

/***( CUDA kernel )*******************************************************/

__global__ void MatMulKernel ( int * MA, int * MB, int * MY, int size )
{
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;

  int Yval = 0;

  /* prevent in excess threads to perform out-of-bounds operations */
  if ( row < size && col < size )
  {
    for ( int i = 0; i < size; i++ )
      Yval += MA[row * size + i] * MB[i * size + col];
  }
  MY[row * size + col] = Yval;
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
      MA[row * size + col] = row * size + col;
      MB[row * size + col] = row * size + col + size * size;
    }
}

/* Print matrix of the given size */
void PrintMatrix ( int *matrix, int size )
{
  int row,
      col;

  for ( row = 0; row < size; row++ )
  {
    for ( col = 0; col < size; col++ )
      printf ( "%8d ", matrix [row * size + col] );
    printf ( "\n" );
  }
  printf ( "\n" );
}

/*--( Main function )----------------------------------------------------*/

int main ( int argc, char *argv[] )
{
  int *MA,
      *MB,
      *MY;
  int *d_MA,
      *d_MB,
      *d_MY;
  int N,
      size,
      BpG,
      BpGx,
      BpGy,
      TpBx,
      TpBy;

  if ( argc != 3 )
  {
    printf ( "Usage: %s N B\n\twhere\tN is the number of rows and cols in the test matrices, and\n\t\tB is the number of GPU blocks\n", argv[0] );
    return ( -1 );
  }

  N = atoi ( argv[1] );
  size = N * N * sizeof(int);

  BpG = atoi ( argv[2] );

  MA = (int *) malloc ( size );
  MB = (int *) malloc ( size );
  MY = (int *) malloc ( size );

  /* Initialise A, B matrices */
  init_matrices ( MA, MB, N );

  if ( N <= 32 )
  {
    printf ( "\n%dx%d A Matrix is \n", N, N );
    PrintMatrix ( MA, N );

    printf ( "\n%dx%d B Matrix is \n", N, N );
    PrintMatrix ( MB, N );
  }

  /* set MY = { {0} } */
  MY = (int *) malloc ( size );
  memset ( (void *) MY, 0, size );

  cudaMalloc ( &d_MA, size );
  cudaMalloc ( &d_MB, size );

  cudaMemcpy ( d_MA, MA, size, cudaMemcpyHostToDevice );
  cudaMemcpy ( d_MB, MB, size, cudaMemcpyHostToDevice );

  cudaMalloc ( &d_MY, size );
  cudaMemcpy ( d_MY, MY, size, cudaMemcpyHostToDevice );

  BpGx = BpGy = BpG;
  dim3 gridSize ( BpGx, BpGy );

  TpBx = ( N + gridSize.x - 1 ) / gridSize.x;
  TpBy = ( N + gridSize.y - 1 ) / gridSize.y;
  dim3 blockSize ( TpBx, TpBy );

  MatMulKernel <<< blockSize, gridSize >>> ( d_MA, d_MB, d_MY, N );

  cudaMemcpy ( MY, d_MY, size, cudaMemcpyDeviceToHost );

  if ( N <= 32 )
  {
    printf ( "\n%dx%d Y Matrix is \n", N, N );
    PrintMatrix ( MY, N );
  }

  cudaFree ( d_MA );
  cudaFree ( d_MB );
  cudaFree ( d_MY );

  free ( MA );
  free ( MB );
  free ( MY );
}
 
