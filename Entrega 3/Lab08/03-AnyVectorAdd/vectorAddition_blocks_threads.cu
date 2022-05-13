/*******************************************************************************
*                                                                              *
*  vectorAddition_blocks_threads.cu -                                          *
*                                                                              *
*   Adds any two vectors on the device (GPU) using several blocks and several  *
*   threads per block                                                          *
*                                                                              *
*                     Departamento de Electronica y Ciencias de la Computacion *
*                     Pontificia Universidad Javeriana - CALI                  *
*                                                                              *
*******************************************************************************/

#include    <stdio.h>
#include    <stdlib.h>

/***( Manifest Constants )************************************************/

#define N                 16 * 1024 * 1024
#define BLOCKS            128
#define THREADS_PER_BLOCK 128

/***( Code to be executed on the device (GPU) )****************************/

/* Add two vectors */
__global__ void add ( int *augend, int *addend, int *result )
{
  int index = blockIdx.x * blockDim.x + threadIdx.x;

  while ( index < N )
  {
    result [index] = augend [index] + addend [index];
    /* shift by the total number of threads in the grid */
    index += BLOCKS * THREADS_PER_BLOCK;
  }
}

/***( Code to be executed on the host (CPU) )******************************/

/*--( Support functions )------------------------------------------------*/

/* Initialise a vector of the given length */
void init_vect ( int *vector, int length )
{
  int i;

  for ( i = 0; i < length; i++ )
    vector[i] = i;
}

/*--( Main function )----------------------------------------------------*/
int main ( void )
{
  int *augend, *addend, *result;        /* host copies of augend, addend, result */
  int *d_augend, *d_addend, *d_result;  /* device copies of augend, addend, result */
  int size = N * sizeof (int);

  /* Allocate space for host copies of augend, addend, result; setup input values */
  augend = (int *) malloc ( size ); init_vect ( augend, N );
  addend = (int *) malloc ( size ); init_vect ( addend, N );
  result = (int *) malloc ( size );

  /* Allocate space for device copies of augend, addend, result */
  cudaMalloc ( (void **) &d_augend, size);
  cudaMalloc ( (void **) &d_addend, size);
  cudaMalloc ( (void **) &d_result, size);

  /* Copy inputs to device */
  cudaMemcpy ( d_augend, augend, size, cudaMemcpyHostToDevice );
  cudaMemcpy ( d_addend, addend, size, cudaMemcpyHostToDevice );

  /* Launch add () kernel on GPU */
  add <<< BLOCKS, THREADS_PER_BLOCK >>> ( d_augend, d_addend, d_result );

  /* Copy result back to host */
  cudaMemcpy ( result, d_result, size, cudaMemcpyDeviceToHost );

  /* verify that the calculation is correct */
  bool success = true;

  for ( int i = 0; i < N; i++ )
  {
    if ( result[i] != ( 2 * i ) )
    {
      printf ( "Aaaargh! Result at element %d (%d) doesn't match golden ref (%d)!\n",
               i, result[i], 2 * i );
      success = false;
    }
  }
  if ( success )
    printf ( "GPU vector addition of %d elements matches golden ref\n", N );

  /* Cleanup */
  cudaFree ( d_augend );
  cudaFree ( d_addend );
  cudaFree ( d_result );

  free ( augend );
  free ( addend );
  free ( result );

  return ( 0 );
}
