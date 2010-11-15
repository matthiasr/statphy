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
title = "`grep 'N = 1,' test-meta.txt|sed 's/, /\\n/g'`"
set key title title
plot "test-0001.testdat" with steps notitle

set origin 0.5, 0.5
title = "`grep 'N = 10,' test-meta.txt|sed 's/, /\\n/g'`"
set key title title
plot "test-0010.testdat" with steps notitle

set origin 0.0, 0.0
title = "`grep 'N = 100,' test-meta.txt|sed 's/, /\\n/g'`"
set key title title
plot "test-0100.testdat" with steps notitle

set origin 0.5, 0.0
title = "`grep 'N = 1000,' test-meta.txt|sed 's/, /\\n/g'`"
set key title title
plot "test-1000.testdat" with steps notitle
