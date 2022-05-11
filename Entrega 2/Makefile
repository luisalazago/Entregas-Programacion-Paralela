#
# Departamento de Electronica y Ciencias de la Computacion
# Facultad de Ingenieria u Ciencias
# Pontificia Universidad Javeriana - CALI
#
# OpenMP Examples
# Sample Makefile
#

#
# Set the compilers and associated flags
#

CC		= mpicc
OMPFLAGS	= -fopenmp
OPTFLAGS	=
DEBUGFLAGS	=
CFLAGS		= -Wall $(OMPFLAGS) $(OPTFLAGS) $(DEBUGFLAGS)
LDFLAGS		= $(OPTFLAGS) $(DEBUGFLAGS) -lm

PROGRAM 	= Parallel_OpenMP_MPI_V2

PROGRAMS        = Parallel_OpenMp_V2 Parallel_OpenMPI_V2 Parallel_OpenMP_MPI Parallel_OpenMp Parallel_OpenMPI
PROCS      	= 16
THREADS		= 8

#
# Targets, in order of appearance
#

all: $(PROGRAMS)

$(PROGRAM): $(PROGRAM).c
	$(CC) $(CFLAGS) $(PROGRAM).c -o $(PROGRAM)

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS) $(PROGRAM)

run:
	@echo "Will run $(PROGRAM) using $(THREADS) threads:"; export OMP_NUM_THREADS=$(THREADS); ./$(PROGRAM)

runa:
	@clear;echo "Will run $(PROGRAM) using $(PROCS) processes:"; mpiexec -n $(PROCS) Parallel_OpenMPI_V2

runtp:
	@clear;echo "Will run $(PROGRAM) using $(PROCS) processes and $(THREADS) threads:"; export OMP_NUM_THREADS=$(THREADS); mpiexec -n $(PROCS) ./$(PROGRAM)

auto_run:
	@echo -e "Will run $(PROGRAM) using `getconf _NPROCESSORS_ONLN` CPUs (logical cores) on host `hostname`:\n`lscpu` "; ./$(PROGRAM)
