#
# Departamento de Electronica y Ciencias de la Computacion
# Pontificia Universidad Javeriana - CALI
#
# CUDA Examples
# Sample Makefile
#

#
# Set the compilers and associated flags
#

CC		= nvcc
OPTFLAGS	=
DEBUGFLAGS	=
CFLAGS		= $(OPTFLAGS) $(DEBUGFLAGS)
LDFLAGS		= $(OPTFLAGS) $(DEBUGFLAGS)

PROGRAMS 	= dist CUDA_dist

#
# Targets, in order of appearance
#

all : $(PROGRAMS)

dist: dist.c
	gcc $(CFLAGS) -std=c99 -Wall dist.c -o dist -lm

CUDA_dist: dist.cu
	$(CC) $(CFLAGS) dist.cu -o CUDA_dist

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS)
