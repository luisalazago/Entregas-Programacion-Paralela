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

CC			= nvcc
OPTFLAGS	=
DEBUGFLAGS	=
CFLAGS		= $(OPTFLAGS) $(DEBUGFLAGS)
LDFLAGS		= $(OPTFLAGS) $(DEBUGFLAGS)

PROGRAMS 	= CUDA_MatrixMultiplication_V1.cu

#
# Targets, in order of appearance
#

all : $(PROGRAMS)

CUDA_Mat: CUDA_MatrixMultiplication_V1.cu
	$(CC) $(CFLAGS) CUDA_MatrixMultiplication_V1.cu -o CUDA_MatrixMultiplication_V1

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS)
