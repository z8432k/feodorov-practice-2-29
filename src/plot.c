#define _GNU_SOURCE
#include "include/plot.h"

static const char *gnuplotFormat = "gnuplot -p -e 'set xlabel \"X\";set ylabel \"Y\";set grid x y;set style line 1 lt rgb \"red\" lw 2.5 pt 6;set style line 2 lt rgb \"blue\" lw 1.5 pt 6 dt 3;set arrow from %.2lf, graph 0 to %.2lf, graph 1 nohead ls 2;set arrow from %.2lf, graph 0 to %.2lf, graph 1 nohead ls 2;plot \"/dev/stdin\" using 1:2 with lines smooth csplines ls 1 t \"\"'";

void plot(GArray *xGVector, GArray *yGVector, double from, double to) {
  char *gnuplotCommand;

  asprintf(&gnuplotCommand, gnuplotFormat, from, from, to, to);

  FILE *gplot = popen(gnuplotCommand, "w");

  gushort i;
  for (i = 0; i < xGVector->len && i < xGVector->len; i++) {
    fprintf(gplot, "%.2lf %.2lf\n", g_array_index(xGVector, double, i), g_array_index(yGVector, double, i));
  }
  fprintf(gplot, "%d", EOF);

  pclose(gplot);
  free(gnuplotCommand);
}
