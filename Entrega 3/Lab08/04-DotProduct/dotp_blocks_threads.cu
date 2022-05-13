/*******************************************************************************
*                                                                              *
*  dotp_blocks_threads.cu -
*
*   Calculates the dot product of two vectors, v1 and v2; the product of
*   pairwise elements is calculated using the GPU and is stored in a third
*   vector, vy. The contents of vy is added using the CPU
*                                                                              *
*                     Departamento de Electronica y Ciencias de la Computacion *
*                     Pontificia Universidad Javeriana - CALI                  *
*                                                                              *
*******************************************************************************/

#include    <stdio.h>
#include    <stdlib.h>

/***( Manifest Constants )************************************************/

#define N                 512
#define BLOCKS            8
#define THREADS_PER_BLOCK 32

/***( Code to be executed on the device (GPU) )****************************/

/* Multiply two vectors */
__global__ void mult_vects ( int *v1, int *v2, int *vy )
{
  int index = blockIdx.x * blockDim.x + threadIdx.x;

  while ( index < N )
  {
    vy [index] = v1 [index] * v2 [index];
    /* shift by the total number of threads in the grid */
    index += BLOCKS * THREADS_PER_BLOCK;
  }
}

/***( Code to be executed on the host (CPU) )******************************/

/*--( Support functions )------------------------------------------------*/

/* Initialise vectors of the given length */
void init_vectors ( int *v1, int *v2, int length )
{
  int i;

  for ( i = 0; i < length; i++ )
  {
    v1[i] = i;
    v2[i] = 2 * i;
  }
}

/*--( Main function )----------------------------------------------------*/
int main ( void )
{
  int *v1, *v2, *vy;        /* host copies of v1, v2, vy */
  int *d_v1, *d_v2, *d_vy;  /* device copies of v1, v2, vy */
  int size = N * sizeof (int);

  /* Allocate space for host copies of v1, v2, vy; setup input values */
  v1 = (int *) malloc ( size );
  v2 = (int *) malloc ( size );
  init_vectors ( v1, v2, N );
  vy = (int *) malloc ( size );

  /* Allocate space for device copies of v1, v2, vy */
  cudaMalloc ( (void **) &d_v1, size);
  cudaMalloc ( (void **) &d_v2, size);
  cudaMalloc ( (void **) &d_vy, size);

  /* Copy inputs to device */
  cudaMemcpy ( d_v1, v1, size, cudaMemcpyHostToDevice );
  cudaMemcpy ( d_v2, v2, size, cudaMemcpyHostToDevice );

  /* Launch mult_vects () kernel on GPU */
  mult_vects <<< BLOCKS, THREADS_PER_BLOCK >>> ( d_v1, d_v2, d_vy );

  /* Copy vy back to host */
  cudaMemcpy ( vy, d_vy, size, cudaMemcpyDeviceToHost );

  /* add the elements in the vy vector */
  int result = 0;
  for ( int i = 0; i < N; i++ )
    result += vy[i];

  /* verify that the calculation is correct */
  bool success = true;
  #define sum_squares(x)  (int) ( (x) * ( (x) + 1 ) * ( 2 * (x) + 1 ) / 6 )
  if ( result != 2 * sum_squares ( N - 1 ) )
    success = false;
  if ( success )
    printf ( "GPU-CPU dot product (%d) matches golden ref (%d)\n", result, 2 * sum_squares ( N - 1 ) );

  /* Cleanup */
  cudaFree ( d_v1 );
  cudaFree ( d_v2 );
  cudaFree ( d_vy );

  free ( v1 );
  free ( v2 );
  free ( vy );

  return ( 0 );
}
