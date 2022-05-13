/*******************************************************************************
*                                                                              *
*  dotp_krnl.cu -                                                              *
*                                                                              *
*   Calculates the dot product of two vectors, v1 and v2                       *
*   Uses shared memory in the GPU to store partial per-block dot products to   *
*   then add up them in the CPU                                                *
*                                                                              *
*                     Departamento de Electronica y Ciencias de la Computacion *
*                     Pontificia Universidad Javeriana - CALI                  *
*                                                                              *
*******************************************************************************/

#include <cuda.h>

/***( Manifest Constants )************************************************/

#define BLOCKS            4
#define THREADS_PER_BLOCK 16

/***( Code to be executed on the device (GPU) )****************************/

__global__ void dotp ( int *v1, int *v2, int vlen, int *vyb )
{
  int index = blockIdx.x * blockDim.x + threadIdx.x;

  /* declare shm_bdp in the shared memory */
  __shared__ int shm_bdp[THREADS_PER_BLOCK];

  int   tdp = 0;
  while ( index < vlen )
  {
    tdp += v1[index] * v2[index];
    index += BLOCKS * THREADS_PER_BLOCK;  // increase by the total number of thread in a grid
  }

  /* set the shm_bdp values */
  shm_bdp[threadIdx.x] = tdp;

  /* synchronise threads in this block */
  __syncthreads();

  /* for reductions, THREADS_PER_BLOCK must be a power of 2 because of the following code */
  int i = THREADS_PER_BLOCK / 2;
  while ( i != 0 )
  {
    if ( threadIdx.x < i )
        shm_bdp[threadIdx.x] += shm_bdp[threadIdx.x + i];
    __syncthreads();
    i /= 2;
  }

  /* thread 0 writes back to the global memory */
  if ( threadIdx.x == 0 )
    vyb[blockIdx.x] = shm_bdp[0];
}

/***( Code to be executed on the host (CPU) )******************************/

/*--( Support functions )------------------------------------------------*/

extern "C" void launch_dotp_kernel ( int *v1, int *v2, int vlen, int *result )
{
  int *vyb;                   /* host copy of vyb */
  int *d_v1, *d_v2, *d_vyb;   /* device copies of v1, v2, vyb */
  int size = vlen * sizeof (int);

  /* Allocate space for host copy of vyb */
  vyb = (int *) malloc ( BLOCKS * sizeof(int) );

  /* Allocate space for device copies of v1, v2, vyb */
  cudaMalloc ( (void **) &d_v1, size );
  cudaMalloc ( (void **) &d_v2, size );
  cudaMalloc ( (void **) &d_vyb, BLOCKS * sizeof(int) );

  /* Copy inputs to device */
  cudaMemcpy ( d_v1, v1, size, cudaMemcpyHostToDevice );
  cudaMemcpy ( d_v2, v2, size, cudaMemcpyHostToDevice );

  /* Launch dotp () kernel on GPU */
  dotp <<< BLOCKS, THREADS_PER_BLOCK >>> ( d_v1, d_v2, vlen, d_vyb );

  /* Copy vyb back to host */
  cudaMemcpy ( vyb, d_vyb, BLOCKS * sizeof(int), cudaMemcpyDeviceToHost );

  /* add the elements in the vyb vector */
  for ( int i = 0; i < BLOCKS; i++ )
    *result += vyb[i];
}
