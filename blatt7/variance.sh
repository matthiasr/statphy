#!/bin/bash
N=100
for (( i=1; $i <= $N; i++ )); do
./aufg21
done | awk -f variance.awk
