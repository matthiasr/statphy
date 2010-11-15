LDFLAGS=-lm
CFLAGS=-O3
GNUPLOT=gnuplot

all: aufg10 aufg10-meta.txt aufg10.pdf

aufg10: aufg10.c

aufg10-meta.txt: aufg10
	./aufg10 -t aufg10 > $@

aufg10.pdf: aufg10.gnuplot aufg10-meta.txt
	$(GNUPLOT) $<

.PHONY: clean distclean
clean:
	rm -f aufg10

distclean: clean
	rm -f *.testdat aufg10-meta.txt aufg10.pdf
