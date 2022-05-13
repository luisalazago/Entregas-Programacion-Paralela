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

PROGRAMS 	= MatMatMult_threads0 MatMatMult_threads1 MatMatMult_blocks_threads

#
# Targets, in order of appearance
#

all : $(PROGRAMS)

MatMatMult_threads0: MatMatMult_threads0.cu
	$(CC) $(CFLAGS) MatMatMult_threads0.cu -o MatMatMult_threads0

MatMatMult_threads1: MatMatMult_threads1.cu
	$(CC) $(CFLAGS) MatMatMult_threads1.cu -o MatMatMult_threads1

MatMatMult_blocks_threads: MatMatMult_blocks_threads.cu
	$(CC) $(CFLAGS) MatMatMult_blocks_threads.cu -o MatMatMult_blocks_threads

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS)
