/*******************************************************************************
*                                                                              *
*  dist.cu - Compute an array of distances from a reference point to each of   *
*            N points uniformly spaced along a line segment                    *
*            Illustrates how to invoke functions from inside the kernel        *
*                                                                              *
*                     Departamento de Electronica y Ciencias de la Computacion *
*                     Pontificia Universidad Javeriana - CALI                  *
*                                                                              *
*******************************************************************************/

#include <stdio.h>

#define N   64
#define TpB 32

/***( support functions to be executed on the GPU )***********************/

__device__ float scale ( int i, int n )
{
  return ( ( (float) i ) / (n - 1) );
}

__device__ float distance ( float x1, float x2 )
{
  return sqrt ( ( (x2 - x1 ) * ( x2 - x1 ) ) );
}

/***( CUDA kernel )*******************************************************/

__global__ void distanceKernel ( float *d_out, float ref, int len )
{
  const int i = blockIdx.x * blockDim.x + threadIdx.x;
  const float x = scale ( i, len );

  d_out[i] = distance ( x, ref );
  printf ( "i = %2d: dist from %f to %f is %f.\n", i, ref, x, d_out[i] );
}

/***( main function )*****************************************************/

int main ( void )
{
  const float ref = 0.5f;
  /* Declare a pointer for an array of floats */
  float *d_out = 0;

  /* Allocate device memory to store the output array */
  cudaMalloc ( &d_out, N * sizeof(float) );

  /* Launch kernel to compute and store distance values */
  distanceKernel <<< N / TpB, TpB >>> ( d_out, ref, N );

  /* Free the memory */
  cudaFree ( d_out );

  return ( 0 );
}
