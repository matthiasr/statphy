all: aufg21 prng-test

aufg21: aufg21.o prng.o

prng-test: prng-test.o prng.o

prng.o: prng.c prng.h

.PHONY: clean

clean:
	rm -f *.o prng-test aufg21
