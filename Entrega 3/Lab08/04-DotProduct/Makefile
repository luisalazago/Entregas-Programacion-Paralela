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

PROGRAMS 	= dotp CUDA_dotp_blocks_threads CUDA_dotp_v1 CUDA_dotp_v2

#
# Targets, in order of appearance
#

all : $(PROGRAMS)

dotp: dotp.c
	gcc $(CFLAGS) -std=c99 -Wall dotp.c -o dotp

CUDA_dotp_blocks_threads: dotp_blocks_threads.cu
	$(CC) $(CFLAGS) dotp_blocks_threads.cu -o CUDA_dotp_blocks_threads

CUDA_dotp_v1: dotp_v1.cu
	$(CC) $(CFLAGS) dotp_v1.cu -o CUDA_dotp_v1

CUDA_dotp_v2: dotp_v2.cu
	$(CC) $(CFLAGS) dotp_v2.cu -o CUDA_dotp_v2

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS)
