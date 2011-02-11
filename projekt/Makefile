LDFLAGS = -lm -O3 -fopenmp
CFLAGS = -g -O3 -fopenmp

all: bin

bin: aufg33

data: aufg33.txt

plots: aufg33.pdf

%.pdf: %.txt
	./plot.sh $< $@

aufg33: aufg33.o

aufg33.txt: aufg33
	./aufg33 > $@

aufg33.o: aufg33.c

.PHONY: clean distclean

.PRECIOUS: aufg33.txt

clean:
	rm -f *.o aufg33

distclean: clean
	rm -f aufg33.txt aufg33.pdf
