set terminal pdf
set output "test.pdf"

set yrange [0.:500]
set xrange [0:1000]
unset key
set size 1.0, 1.0
set origin 0.0, 0.0
set multiplot
set size 0.5, 0.5

set origin 0.0, 0.5
set key title "N = 1"
plot "test-0001.testdat" with steps notitle

set origin 0.5, 0.5
set key title "N = 10"
plot "test-0010.testdat" with steps notitle

set origin 0.0, 0.0
set key title "N = 100"
plot "test-0100.testdat" with steps notitle

set origin 0.5, 0.0
set key title "N = 1000"
plot "test-1000.testdat" with steps notitle
