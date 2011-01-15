CPPFLAGS = -DMETROPOLIS_DELTA=0.01
LDFLAGS = -lm -O3
CFLAGS = -g -O3

all: bin data plots

bin: aufg26

data: aufg26.txt

plots: aufg26.pdf

%.pdf: %.txt
	./plot.sh $< $@

aufg26: aufg26.o prng.o metropolis.o

aufg26.txt: aufg26
	./aufg26 > $@

aufg26.o: aufg26.c metropolis.h prng.h

prng.o: prng.c prng.h

metropolis.o: metropolis.c metropolis.h prng.h

.PHONY: clean distclean

.PRECIOUS: aufg26.txt

clean:
	rm -f *.o aufg26

distclean: clean
	rm -f aufg26.txt aufg26.pdf
