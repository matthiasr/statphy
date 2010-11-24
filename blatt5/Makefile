CFLAGS=-O3
CPPFLAGS=-DP_STEP=0.01

all: aufg16-test aufg16

aufg16-test: aufg16-test.o aufg16-functions.o

aufg16: aufg16.o aufg16-functions.o

aufg16.txt: aufg16
	./aufg16 > $@


.PHONY: clean distclean

clean:
	rm -f aufg16 aufg16-test *.o

distclean: clean
	rm -f aufg16.txt
