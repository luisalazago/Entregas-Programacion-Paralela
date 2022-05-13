/*******************************************************************************
*                                                                              *
*  dotp_v2.cu -                                                                *
*                                                                              *
*   Calculates the dot product of two vectors, v1 and v2; the product of
*   pairwise elements is calculated using the GPU and is stored in a third 
*   vector, result
*   The contents of result is added using the CPU
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

__global__ void dotp ( int *v1, int *v2, int *result )
{
  int index = blockIdx.x * blockDim.x + threadIdx.x;

  /* declare shm_tdp in the shared memory */
  __shared__ int shm_tdp[THREADS_PER_BLOCK];

  shm_tdp[threadIdx.x] = 0;
  while ( index < N )
  {
    shm_tdp[threadIdx.x] += v1[index] * v2[index];
    index += BLOCKS * THREADS_PER_BLOCK;  // increase by the total number of thread in a grid
  }

  /* synchronise threads in this block */
  __syncthreads();

  /* thread 0 reduces */
  if ( threadIdx.x == 0 )
  {
    int bdp = 0;
    for ( int i = 0; i < THREADS_PER_BLOCK; i++ )
    {
      bdp += shm_tdp[i];
    }
    atomicAdd ( result, bdp );
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
  int *v1, *v2, *result;        /* host copies of v1, v2, result */
  int *d_v1, *d_v2, *d_result;  /* device copies of v1, v2, result */
  int size = N * sizeof (int);

  /* Allocate space for host copies of v1, v2, result; setup input values */
  v1 = (int *) malloc ( size );
  v2 = (int *) malloc ( size );
  init_vectors ( v1, v2, N );
  result = (int *) malloc ( sizeof(int) );

  /* Allocate space for device copies of v1, v2, result */
  cudaMalloc ( (void **) &d_v1, size );
  cudaMalloc ( (void **) &d_v2, size );
  cudaMalloc ( (void **) &d_result, sizeof(int) );

  /* Copy inputs to device */
  cudaMemcpy ( d_v1, v1, size, cudaMemcpyHostToDevice );
  cudaMemcpy ( d_v2, v2, size, cudaMemcpyHostToDevice );
  *result = 0;
  cudaMemcpy ( d_result, result, sizeof(int), cudaMemcpyHostToDevice );

  /* Launch dotp () kernel on GPU */
  dotp <<< BLOCKS, THREADS_PER_BLOCK >>> ( d_v1, d_v2, d_result );

  /* Copy result back to host */
  cudaMemcpy ( result, d_result, sizeof(int), cudaMemcpyDeviceToHost );

  /* verify that the calculation is correct */
  bool success = true;
  #define sum_squares(x)  (int) ( (x) * ( (x) + 1 ) * ( 2 * (x) + 1 ) / 6 )
  if ( *result != 2 * sum_squares ( N - 1 ) )
    success = false;
  if ( success )
    printf ( "GPU dot product (%d) matches golden ref (%d)\n", *result, 2 * sum_squares ( N - 1 ) );

  /* Cleanup */
  cudaFree ( d_v1 );
  cudaFree ( d_v2 );
  cudaFree ( d_result );

  free ( v1 );
  free ( v2 );
  free ( result );

  return ( 0 );
}
