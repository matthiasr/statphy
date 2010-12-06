#!/bin/sh

gnuplot << EOF
set terminal pdf
set output "$2"

set xlabel 'p'
set ylabel 'relative Häufigkeit'

set xrange [0:1]

set key right bottom

plot '$1' index 0 with steps title "N=10", \
     '$1' index 1 with steps title "N=100", \
     '$1' index 2 with steps title "N=1000"
EOF
