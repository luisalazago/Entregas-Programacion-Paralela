#
# Departamento de Electronica y Ciencias de la Computacion
# Pontificia Universidad Javeriana - CALI
#
# MPI + CUDA Example
# Sample Makefile
#

PROGRAMS 	= MPI_dotp

#
# Targets, in order of appearance
#

all : $(PROGRAMS)

MPI_dotp: dotp.o
	mpicc dotp.o -o MPI_dotp

dotp.o: dotp.c
	mpicc -Wall -std=c99 -c dotp.c

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS)
