/*******************************************************************************
*                                                                              *
*  dotp.c -                                                                    *
*                                                                              *
*   Dot product using two compute nodes (MPI + CUDA)
*                                                                              *
*                     Departamento de Electronica y Ciencias de la Computacion *
*                     Pontificia Universidad Javeriana - CALI                  *
*                                                                              *
*******************************************************************************/

#include    <stdio.h>
#include    <stdlib.h>
#include    <stdbool.h>
#include    <mpi.h>

/***( Manifest Constants )************************************************/

#define PROC0       0
#define PROC1       1

#define N           512
#define VECT_SIZE   (N / 2)

#define VECT1_TAG   1
#define VECT2_TAG   2
#define RES_TAG     3

/***( Function Prototypes )***********************************************/

void launch_dotp_kernel ( int *v1, int *v2, int vlen, int *result );

/***( Program Code )******************************************************/

/*--( Support functions )------------------------------------------------*/

/* Initialise vectors of the given length */
void init_vectors ( int *v1, int *v2, int vlen )
{
  int i;

  for ( i = 0; i < vlen; i++ )
  {
    v1[i] = i;
    v2[i] = 2 * i;
  }
}

/*--( Main function )----------------------------------------------------*/

int main ( int argc, char *argv[] )
{
  int my_rank,
      p;

  MPI_Init ( &argc, &argv );
  MPI_Comm_rank ( MPI_COMM_WORLD, &my_rank );
  MPI_Comm_size ( MPI_COMM_WORLD, &p );

  /* Get the name of the processor */
  char proc_name [MPI_MAX_PROCESSOR_NAME];
  int  name_len;
  MPI_Get_processor_name ( proc_name, &name_len );
  
  int  *v1, *v2;
  int  size;
  int  my_result,
       half_result,
       result;

  if ( my_rank == PROC0 )
  {
    printf ( "I am process %d of %d running on host %s\n", my_rank, p, proc_name );

    /* Allocate space for local copies of v1, v2; setup input values */
    size = N;
    v1 = (int *) malloc ( size * sizeof (int) );
    v2 = (int *) malloc ( size * sizeof (int) );
    init_vectors ( v1, v2, N );

    /* Send upper halves to PROC1 */
    size = VECT_SIZE;
    MPI_Send ( &v1[size], size, MPI_INT, PROC1, VECT1_TAG, MPI_COMM_WORLD );
    MPI_Send ( &v2[size], size, MPI_INT, PROC1, VECT2_TAG, MPI_COMM_WORLD );
  }
  else
  if ( my_rank == PROC1 )
  {
    printf ( "I am process %d of %d running on host %s\n", my_rank, p, proc_name );
    
    MPI_Status status;

    MPI_Probe ( PROC0, VECT1_TAG, MPI_COMM_WORLD, &status );
    MPI_Get_count ( &status, MPI_INT, &size );

    /* Allocate space for local copies of v1, v2 */
    v1 = (int *) malloc ( size * sizeof (int) );
    v2 = (int *) malloc ( size * sizeof (int) );

    /* Receive upper halves from PROC2 */
    MPI_Recv ( v1, size, MPI_INT, PROC0, VECT1_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    MPI_Recv ( v2, size, MPI_INT, PROC0, VECT2_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
  }

  /* Launch dotp_kernel on GPU */
  my_result = 0;
  launch_dotp_kernel ( v1, v2, size, &my_result );

  if ( my_rank == PROC1 )
  {
    MPI_Send ( &my_result, 1, MPI_INT, PROC0, RES_TAG, MPI_COMM_WORLD );
  }
  else
  if ( my_rank == PROC0 )
  {
    MPI_Recv ( &half_result, 1, MPI_INT, PROC1, RES_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    result = my_result + half_result;

    /* verify that the calculation is correct */
    bool success = true;
    #define sum_squares(x)  (int) ( (x) * ( (x) + 1 ) * ( 2 * (x) + 1 ) / 6 )
    if ( result != 2 * sum_squares ( N - 1 ) )
      success = false;
    if ( success )
      printf ( "MPI+CUDA dot product (%d) matches golden ref (%d)\n",
               result, 2 * sum_squares ( N - 1 ) );
  }

  /* Cleanup */
  free ( v1 );
  free ( v2 );

  /* Shutdown MPI runtime */
  MPI_Finalize ();
  return ( 0 );
}
