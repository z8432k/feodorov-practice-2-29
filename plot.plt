#! /usr/bin/gnuplot -persist
set xlabel "X"
set ylabel "Y"
set grid x y
set style line 1 lt rgb "red" lw 2.5 pt 6
set style line 2 lt rgb "blue" lw 1.5 pt 6 dt 3
set arrow from 3.5, graph 0 to 3.5, graph 1 nohead ls 2
set arrow from 7, graph 0 to 7, graph 1 nohead ls 2
plot "data/data" using 1:2 with lines smooth csplines ls 1 t ""

