CFLAGS=-O3

all: aufg16-test aufg16

aufg16-test: aufg16-test.o aufg16-functions.o

aufg16: aufg16.o aufg16-functions.o

aufg16.txt: aufg16
	./aufg16 > $@

aufg16-hiq.txt: aufg16
	./aufg16 0.01 > $@

aufg16-vhq.txt: aufg16
	./aufg16 0.001 > $@

.PRECIOUS: aufg16.txt aufg16-hiq.txt aufg16-vhq.txt


.PHONY: clean distclean

clean:
	rm -f aufg16 aufg16-test *.o

distclean: clean
	rm -f aufg16.txt
