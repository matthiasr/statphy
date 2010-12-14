CPPFLAGS = -DMETROPOLIS_DELTA=0.01

aufg23: aufg23.o prng.o metropolis.o

aufg23.o: aufg23.c metropolis.h prng.h

prng.o: prng.c prng.h

metropolis.o: metropolis.c metropolis.h prng.h

clean:
	rm -f *.o

distclean: clean
