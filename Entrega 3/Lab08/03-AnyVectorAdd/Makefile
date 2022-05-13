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

PROGRAMS 	= vectorAddition CUDA_vectorAddition_blocks_threads vectorAddition_t CUDA_vectorAddition_blocks_threads_t CUDA_vectorAddition_scaleblocks_threads_t

#
# Targets, in order of appearance
#

all : $(PROGRAMS)

vectorAddition: vectorAddition.c
	gcc $(CFLAGS) -Wall vectorAddition.c -o vectorAddition

vectorAddition_t: vectorAddition_t.c timer.c
	gcc $(CFLAGS) -Wall -c vectorAddition_t.c; gcc $(CFLAGS) -Wall -c timer.c; gcc $(CFLAGS) vectorAddition_t.o timer.o -o vectorAddition_t

CUDA_vectorAddition_blocks_threads: vectorAddition_blocks_threads.cu
	$(CC) $(CFLAGS) vectorAddition_blocks_threads.cu -o CUDA_vectorAddition_blocks_threads

CUDA_vectorAddition_blocks_threads_t: vectorAddition_blocks_threads_t.cu
	$(CC) $(CFLAGS) vectorAddition_blocks_threads_t.cu -o CUDA_vectorAddition_blocks_threads_t

CUDA_vectorAddition_scaleblocks_threads_t: vectorAddition_scaleblocks_threads_t.cu
	$(CC) $(CFLAGS) vectorAddition_scaleblocks_threads_t.cu -o CUDA_vectorAddition_scaleblocks_threads_t

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS)
