#!/bin/sh

gnuplot <<EOF
set terminal pdf size 21cm,29.7cm
set output "$2"

unset key
set size 1.0, 1.0
set origin 0.0, 0.0
set multiplot
set size 1.0,0.5

set origin 0.0, 0.5
set title "Dichteverteilung"
set dgrid3d 100
set contour base
splot "$1" index 0 using 1:2:3 with lines

set origin 0.0,0.0
set title "Paarkorrelationsfunktion"
plot "$1" index 1 using 1:2 with steps
EOF
