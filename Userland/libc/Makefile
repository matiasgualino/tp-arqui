include ../../Makefile.variables
include ../Makefile.inc

HEADERS=-I../../headers -I./include
LIBRARIES=-L../syscalls/lib

all: libc

libc:
	$(GCC) $(GCCFLAGS) $(HEADERS) $(LIBRARIES) -lsyscalls -c libc.c
	$(AR) rcs lib/libc.a libc.o

clean:
	rm -rf *.o liblib*.a

.PHONY: all clean
