set terminal  png size 500,350 font 'Verdana, 10'
set output 'img.png'
set ylabel "Величина, dB (0дБ=1В)"
set yrange [90:140]
set mytics (5)
set grid xtics ytics mytics
plot '1.txt'using 1:2 with lines lw 2 lt rgb 'red',\
'1.txt'using 1:3  with lines lw 2 lt rgb 'green',\
'' u 1:4 w lines lw 2 lt rgb 'blue'
