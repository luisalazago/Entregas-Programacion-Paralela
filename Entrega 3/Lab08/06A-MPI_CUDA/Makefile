#
# Departamento de Electronica y Ciencias de la Computacion
# Pontificia Universidad Javeriana - CALI
#
# MPI + CUDA Example
# Sample Makefile
#

PROGRAMS 	= MPI_CUDA_dotp

#
# Targets, in order of appearance
#

all : $(PROGRAMS)

MPI_CUDA_dotp: dotp.o dotp_krnl.o
	nvcc dotp.o dotp_krnl.o -o MPI_CUDA_dotp -I/usr/lib64/mpi/gcc/openmpi/include -L/usr/lib64/mpi/gcc/openmpi/lib64 -lmpi
#	mpicc dotp.o dotp_krnl.o -o MPI_CUDA_dotp -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -lcudart
# -lculibos -lpthread -lrt -ldl

dotp.o: dotp.c
	mpicc -Wall -std=c99 -c dotp.c

dotp_krnl.o: dotp_krnl.cu
	nvcc -c dotp_krnl.cu

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS)
