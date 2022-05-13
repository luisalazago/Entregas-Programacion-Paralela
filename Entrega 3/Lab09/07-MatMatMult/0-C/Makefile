#
# Departamento de Electronica y Ciencias de la Computacion
# Facultad de Ingenieria y Ciencias
# Pontificia Universidad Javeriana - CALI
#
# C Examples
# Sample Makefile
#

#
# Set the compilers and associated flags
#

CC		= gcc
OPTFLAGS	=
DEBUGFLAGS	=
CFLAGS		= -Wall $(OPTFLAGS) $(DEBUGFLAGS)
LDFLAGS		= $(OPTFLAGS) $(DEBUGFLAGS)

PROGRAMS	= MatMatMult0 MatMatMult1 MatMatMult2 MatMatMult3 MatMatMult4 MatMatMult5 MatMatMult6 MatMatMult7 MatMatMult8 MatMatMult9

#
# Targets, in order of appearance
#

all: $(PROGRAMS)

MatMatMult0: MatMatMult0.c
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult0.c -o MatMatMult0 -lm

MatMatMult1: MatMatMult1.c
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult1.c -o MatMatMult1 -lm

MatMatMult2: MatMatMult2.c
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult2.c -o MatMatMult2 -lm

MatMatMult3: MatMatMult3.c
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult3.c -o MatMatMult3 -lm

MatMatMult4: MatMatMult4.c
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult4.c -o MatMatMult4 -lm

MatMatMult5: MatMatMult5.c
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult5.c -o MatMatMult5 -lm

MatMatMult6: MatMatMult6.o timer.o
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult6.o timer.o -o MatMatMult6 -lm

MatMatMult7: MatMatMult7.o timer.o
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult7.o timer.o -o MatMatMult7 -lm

MatMatMult8: MatMatMult8.c
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult8.c -o MatMatMult8

MatMatMult9: MatMatMult9.c
	$(CC) -std=gnu99 $(CFLAGS) MatMatMult9.c -o MatMatMult9

MatMatMult6.o: MatMatMult6.c
	$(CC) $(CFLAGS) -std=gnu99 -c MatMatMult6.c

MatMatMult7.o: MatMatMult7.c
	$(CC) $(CFLAGS) -std=gnu99 -c MatMatMult7.c

#
# Other nice thingys
#

clean:
	rm -f *% *~ core *.o $(PROGRAMS)
