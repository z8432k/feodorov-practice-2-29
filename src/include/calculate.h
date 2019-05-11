#include <glib.h>
#include <ncurses.h>
#include "../integral/include/integral.h"

typedef struct {
  GArray *xGVector;
  GArray *yGVector;
  double from;
  double to;
} Data_t;


void calculate();
