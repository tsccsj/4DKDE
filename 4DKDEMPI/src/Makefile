GCC= g++
MPICC= mpicxx

all: 4DKDE

io.o: io.c
	$(GCC) -o $@ -c $<
KDEcalc.o: KDEcalc.c
	$(GCC) -o $@ -c $<
4DKDE.o: 4DKDE.c
	$(MPICC) -o $@ -c $<
4DKDE: 4DKDE.o io.o KDEcalc.o
	$(MPICC) -o ../$@ $+

clean: 
	rm -f *.o ../4DKDE
