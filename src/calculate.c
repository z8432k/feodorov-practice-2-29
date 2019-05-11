#include "include/calculate.h"

static void fillVector(GArray *vector, const guint limit);
static Data_t inputData();

void calculate() {
  Data_t data = inputData();

  integralError error;
  double result = integral(data.xGVector, data.yGVector, data.from, data.to, &error);

  GArray *errorStrings = integral_strerror(error);

  if (result < 0) {
    printw("ERROR OCCURED: %s\n", strerror(errno));

    gushort i;
    for (i = 0; i < errorStrings->len; i++) {
      printw("\t- %s\n", g_array_index(errorStrings, char *, i));
    }
  }
  else {
    printw("\n\nResult: %f\n\n", result);
  }

  integral_error_free(errorStrings);
  g_array_free(data.yGVector, TRUE);
  g_array_free(data.xGVector, TRUE);
}



static void fillVector(GArray *vector, const guint limit) {
  double value = 0;

  while ((limit == 0 || vector->len < limit) && scanw("%lf", &value) != EOF) {
    g_array_append_val(vector, value);
  }
}

static Data_t inputData() {
  size_t doubleSize = sizeof(double);

  Data_t data = {
    g_array_new(FALSE, FALSE, doubleSize),
    g_array_new(FALSE, FALSE, doubleSize),
    0,
    0
  };

  clear();
  printw("Input function values one by one. Press enter after each. Press enter for finish: \n");
  fillVector(data.yGVector, 0);

  printw("Ok.\n");
  printw("Your has entered %d function values.\nNow enter %d argument values:\n", data.yGVector->len, data.yGVector->len);
  fillVector(data.xGVector, data.yGVector->len);

  printw("Ok.\n");
  printw("Now enter buttom integration limit:\n");
  scanw("%lf", &data.from);

  printw("Ok.\n");
  printw("Now enter top integration limit:\n");
  scanw("%lf", &data.to);

  return data;
}
