#include "include/demo.h"

static const double xVector[DEMO_ARR_SIZE] = { 3,     3.5,  4,    4.5,  5,    5.5,  6,    6.5,  7,    7.5 };
static const double yVector[DEMO_ARR_SIZE] = { 3.27,  3.53, 3.21, 2.34, 1.5,  1.27, 1.67, 2.18, 2.18, 1.53 };
static const double from = 3.5;
static const double to   = 7;

void demonstration() {
  size_t doubleSize = sizeof(double);

  GArray *xGVector = g_array_new(FALSE, FALSE, doubleSize);
  GArray *yGVector = g_array_new(FALSE, FALSE, doubleSize);

  printw("Demo:\n");

  gushort i;
  for (i = 0; i < DEMO_ARR_SIZE; i++) {
    g_array_append_val(xGVector, xVector[i]);
    g_array_append_val(yGVector, yVector[i]);
  }

  integralError error;
  double result = integral(xGVector, yGVector, from, to, &error);

  GArray *errorStrings = integral_strerror(error);

  if (result < 0) {
    printw("ERROR OCCURED: %s\n", strerror(errno));

    gushort i;
    for (i = 0; i < errorStrings->len; i++) {
      printw("\t- %s\n", g_array_index(errorStrings, char *, i));
    }
  }
  else {
    printw("Demo result: %f\n", result);
  }

  integral_error_free(errorStrings);
}
