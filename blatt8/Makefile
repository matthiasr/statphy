CPPFLAGS = -DMETROPOLIS_DELTA=0.01
LDFLAGS = -lm
CFLAGS = -g

all: bin data

bin: aufg23

data: aufg23.txt

aufg23: aufg23.o prng.o metropolis.o

aufg23.txt: aufg23
	./aufg23 > $@

aufg23.o: aufg23.c metropolis.h prng.h

prng.o: prng.c prng.h

metropolis.o: metropolis.c metropolis.h prng.h

.PHONY: clean distclean

.PRECIOUS: aufg23.txt

clean:
	rm -f *.o aufg23

distclean: clean
	rm -f aufg23.txt
