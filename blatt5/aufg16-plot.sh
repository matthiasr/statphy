#!/bin/sh

gnuplot << EOF
set terminal pdf
set output "$2"

set xlabel 'p'
set ylabel 'relative Häufigkeit'

set xrange [0:1]

# cumulative gaussian distribution
set fit logfile '$1.log'
f_10(x)=(1+erf((x-x0_10)/(sqrt(2)*s_10)))/2
f_100(x)=(1+erf((x-x0_100)/(sqrt(2)*s_100)))/2
f_1000(x)=(1+erf((x-x0_1000)/(sqrt(2)*s_1000)))/2

fit f_10(x) '$1' index 0 using 1:2 via x0_10,s_10
fit f_100(x) '$1' index 1 using 1:2 via x0_100,s_100
fit f_1000(x) '$1' index 2 using 1:2 via x0_1000,s_1000

set samples 1000
set key right bottom

plot '$1' index 0 with steps title "N=10", \
     f_10(x) with lines title "Cumulative Gauss for N=10", \
     '$1' index 1 with steps title "N=100", \
     f_100(x) with lines title "Cumulative Gauss for N=100", \
     '$1' index 2 with steps title "N=1000", \
     f_1000(x) with lines title "Cumulative Gauss for N=1000"
EOF
