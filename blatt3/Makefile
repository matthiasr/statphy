LDFLAGS=-lm
CFLAGS=-O3

all: aufg10 test-meta.txt

aufg10: aufg10.c

test-meta.txt: aufg10
	./aufg10 -t > $@

.PHONY: clean distclean
clean:
	rm -f aufg10

distclean: clean
	rm -f *.testdat test-meta.txt
