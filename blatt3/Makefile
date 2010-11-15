LDFLAGS=-lm
CFLAGS=-O3
GNUPLOT=gnuplot

all: aufg10 test-meta.txt test.pdf

aufg10: aufg10.c

test-meta.txt: aufg10
	./aufg10 -t > $@

test.pdf: test.gnuplot test-meta.txt
	$(GNUPLOT) $<

.PHONY: clean distclean
clean:
	rm -f aufg10

distclean: clean
	rm -f *.testdat test-meta.txt test.pdf
