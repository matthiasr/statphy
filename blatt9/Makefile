LDFLAGS = -lm -O3
CFLAGS = -g -O3

all: bin data plots

bin: aufg26

data: aufg26.txt

plots: aufg26.pdf

%.pdf: %.txt
	./plot.sh $< $@

aufg26: aufg26.o

aufg26.txt: aufg26
	./aufg26 > $@

aufg26.o: aufg26.c

.PHONY: clean distclean

.PRECIOUS: aufg26.txt

clean:
	rm -f *.o aufg26

distclean: clean
	rm -f aufg26.txt aufg26.pdf
