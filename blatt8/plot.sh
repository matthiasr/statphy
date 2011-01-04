#!/bin/sh

gnuplot <<EOF
set terminal pdf size 10,6
set yrange [0:0.4]
set output "$2"

unset key
set size 1.0, 1.0
set origin 0.0, 0.0
set multiplot
set size 0.5, 0.5

set origin 0.0, 0.5
set key title "N = 1"
plot "$1" index 0 with steps title "lambda = 1.5", "" index 1 with steps title "lambda = 2", "" index 2 with steps title "lambda = 5", "" index 3 with steps title "lambda = 10"

set origin 0.5, 0.5
set key title "N = 3"
plot "$1" index 4 with steps title "lambda = 2.598076", "" index 5 with steps title "lambda = 3.464102", "" index 6 with steps title "lambda = 8.660254", "" index 7 with steps title "lambda = 17.320508"

set origin 0.0, 0.0
set key title "N = 10"
plot "$1" index 10 with steps title "lambda = 15.811388", "" index 11 with steps title "lambda = 31.622777"

set origin 0.5, 0.0
set key title "N = 100"
plot "$1" index 15 with steps title "lambda = 100"

EOF
