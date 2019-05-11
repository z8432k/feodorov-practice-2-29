#include "include/plot.h"

void plot(GArray *xGVector, GArray *yGVector, double from, double to) {
  FILE *gplot = popen("gnuplot -p -e 'plot \"/dev/stdin\" using 1:2 with lines'", "w");

  gushort i;
  for (i = 0; i < xGVector->len && i < xGVector->len; i++) {
    fprintf(gplot, "%.2lf %.2lf\n", g_array_index(xGVector, double, i), g_array_index(yGVector, double, i));
  }

  fprintf(gplot, "%d", EOF);

  pclose(gplot);
}
