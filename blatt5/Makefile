CFLAGS=-O3

all: bin data plots

bin: aufg16-test aufg16 aufg16-periodic

data: aufg16.txt aufg16-hiq.txt aufg16-vhq.txt aufg16-periodic.txt aufg16-periodic-hiq.txt aufg16-periodic-vhq.txt

plots: aufg16.pdf aufg16-hiq.pdf aufg16-vhq.pdf aufg16-periodic.pdf aufg16-periodic-hiq.pdf aufg16-periodic-vhq.pdf aufg16-nofit.pdf aufg16-hiq-nofit.pdf aufg16-vhq-nofit.pdf aufg16-periodic-nofit.pdf aufg16-periodic-hiq-nofit.pdf aufg16-periodic-vhq-nofit.pdf

aufg16-test: aufg16-test.o aufg16-functions.o

aufg16: aufg16.o aufg16-functions.o

aufg16-periodic: aufg16.o aufg16-periodic.o

aufg16.txt: aufg16
	./aufg16 > $@

aufg16-hiq.txt: aufg16
	./aufg16 0.01 > $@

aufg16-vhq.txt: aufg16
	./aufg16 0.001 > $@

aufg16-periodic.txt: aufg16-periodic
	./aufg16-periodic > $@

aufg16-periodic-hiq.txt: aufg16-periodic
	./aufg16-periodic 0.01 > $@

aufg16-periodic-vhq.txt: aufg16-periodic
	./aufg16-periodic 0.001 > $@

%-nofit.pdf: %.txt
	./aufg16-plot-nofit.sh $< $@ 2> /dev/null

%.pdf: %.txt
	rm -f `basename $@ .pdf`.log
	./aufg16-plot.sh $< $@ 2> /dev/null

.PRECIOUS: aufg16.txt aufg16-hiq.txt aufg16-vhq.txt aufg16-periodic.txt aufg16-periodic-hiq.txt aufg16-periodic-vhq.txt


.PHONY: clean distclean

clean:
	rm -f aufg16 aufg16-periodic aufg16-test *.o

distclean: clean
	rm -f aufg16.txt aufg16-hiq.txt aufg16-vhq.txt
	rm -f aufg16.pdf aufg16-hiq.pdf aufg16-vhq.pdf
	rm -f aufg16-nofit.pdf aufg16-hiq-nofit.pdf aufg16-vhq-nofit.pdf
	rm -f aufg16.log aufg16-hiq.log aufg16-vhq.log
	rm -f aufg16-periodic.txt aufg16-periodic-hiq.txt aufg16-periodic-vhq.txt
	rm -f aufg16-periodic.pdf aufg16-periodic-hiq.pdf aufg16-periodic-vhq.pdf
	rm -f aufg16-periodic-nofit.pdf aufg16-periodic-hiq-nofit.pdf aufg16-periodic-vhq-nofit.pdf
	rm -f aufg16-periodic.pdf.log aufg16-periodic-hiq.pdf.log aufg16-periodic-vhq.pdf.log
