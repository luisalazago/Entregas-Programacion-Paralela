#
# Departamento de Electronica y Ciencias de la Computacion
# Facultad de Ingenieria y Ciencias
# Pontificia Universidad Javeriana - CALI
#
# OpenMP Examples
# Sample Makefile
#

#
# Set the compilers and associated flags
#

CC		= gcc
OMPFLAGS        = -fopenmp
OPTFLAGS        =
DEBUGFLAGS      =
CFLAGS          = -Wall $(OMPFLAGS) $(OPTFLAGS) $(DEBUGFLAGS)
LDFLAGS         = $(OPTFLAGS) $(DEBUGFLAGS)

PROGRAM 	= MatMatMult0

PROGRAMS	= MatMatMult0 MatMatMult1

THREADS         = 2

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
	rm -f *% *~ core *.o $(PROGRAMS)

run:
	@echo "Will run $(PROGRAM) using $(THREADS) threads:"; export OMP_NUM_THREADS=$(THREADS); ./$(PROGRAM)

auto_run:
	@echo -e "Will run $(PROGRAM) using `getconf _NPROCESSORS_ONLN` CPUs (logical cores) on host `hostname`:\n`lscpu` "; ./$(PROGRAM)
