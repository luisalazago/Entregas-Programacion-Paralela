#include <stdio.h>
#include <cuda.h>

#define N 16
#define MAT_COLS N
#define MAT_ROWS N
#define TILES 4

#define MAX_THREADS 512
#define BLOCKS 4
#define THREADS_PER_BLOCK 32

/***( CUDA kernel )*******************************************************/

__global__ void MatMulKernel(int *MA, int *MB, int *MY)
{
    // Initialise the shared matrix.
    __shared__ int shared_matrix[THREADS_PER_BLOCK][THREADS_PER_BLOCK];

    // Calculate the rows and cols of the matrix.
    int row = blockIdx.y * TILES + threadIdx.y;
    int col = blockIdx.x * TILES + threadIdx.x;

    int val = 0;

    // Prevent in excess threads to perform out-of-bounds operations.
    if (row < N && col < N)
    {
        for (int i = 0; i < N; i++)
            val += MA[row * N + i] * MB[i * N + col]; // Multiply the rows of the first matrix and the columns of the second matrix.
    }

    // Insert into the shared memory the product calculated before.
    shared_matrix[threadIdx.y][threadIdx.x] = val;
    __syncthreads();

    // We verify the size of the rows and columns exist in the matrix.
    if(row < N && col < N) {
        // In the final matrix we insert the values of the shared memory, this optimize the efficiency of the data pass.
        MY[((blockIdx.y * TILES + threadIdx.y) * N) + (blockIdx.x * TILES) + threadIdx.x] = shared_matrix[threadIdx.y][threadIdx.x];
    }
}

/*--( Support functions )------------------------------------------------*/

/* Initialise matrices of the given size */
void llenar(int *matrix, int fil, int col, bool a){
    /*
    Functions that initiallize the matrix that will be used to
    calculate the dot product
    Inputs:
        Matrix: is a matrix of FILxCOL
    Outputs
        Matrix: is a matrix of FILxCOL

    */
    int i, j;
    if(a) {
        for(i = 0; i < fil; ++i){
            for(j = 0; j < col; ++j) {
              matrix[(i*col)+j] = 1;
            }
        }
    }
    else {
        for(i = 0; i < fil; ++i){
            for(j = 0; j < col; ++j) {
              matrix[(i*col)+j] = 2;
            }
        }
    }
}

/* Print matrix of the given size */
void PrintMatrix(int *matrix)
{
    int row,
        col;

    for (row = 0; row < N; row++)
    {
        for (col = 0; col < N; col++)
            printf("%7d ", matrix[row * N + col]);
        printf("\n");
    }
    printf("\n");
}

/*--( Main function )----------------------------------------------------*/

int main(void)
{
    int *MA,
        *MB,
        *MY;
    int *d_MA,
        *d_MB,
        *d_MY;

    int size = N * N * sizeof(int);

    MA = (int *)malloc(size);
    MB = (int *)malloc(size);
    MY = (int *)malloc(size);

    // Initialise A, B matrices
    llenar(MA, N, N, true);
    llenar(MB, N, N, false);

    printf("\n%dx%d A matrix is \n", N, N);
    PrintMatrix(MA);

    printf("\n%dx%d B matrix is \n", N, N);
    PrintMatrix(MB);

    // Initialise the Y (final) matrix.
    MY = (int *)malloc(size);
    memset((void *)MY, 0, size);

    // Allocate in the DRAM the space and data from the CPU to the GPU.
    cudaMalloc(&d_MA, size);
    cudaMalloc(&d_MB, size);

    cudaMemcpy(d_MA, MA, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_MB, MB, size, cudaMemcpyHostToDevice);

    cudaMalloc(&d_MY, size);
    cudaMemcpy(d_MY, MY, size, cudaMemcpyHostToDevice);

    dim3 BlocksPerGrid(1, 1);
    dim3 ThreadsPerBlock(N, N);
    if (N * N > MAX_THREADS)
    {
        ThreadsPerBlock.x = MAX_THREADS;
        ThreadsPerBlock.y = MAX_THREADS;
        BlocksPerGrid.x = ceil((double)N / (double)ThreadsPerBlock.x);
        BlocksPerGrid.y = ceil((double)N / (double)ThreadsPerBlock.y);
    }

    MatMulKernel<<<BlocksPerGrid, ThreadsPerBlock>>>(d_MA, d_MB, d_MY);

    // Get the data from the Y matrix filled in the kernel.
    cudaMemcpy(MY, d_MY, size, cudaMemcpyDeviceToHost);

    printf("\n%dx%d Y matrix is \n", N, N);
    PrintMatrix(MY);

    cudaFree(d_MA);
    cudaFree(d_MB);
    cudaFree(d_MY);

    free(MA);
    free(MB);
    free(MY);
}
