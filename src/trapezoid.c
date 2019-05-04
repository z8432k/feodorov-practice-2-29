#include <stdio.h>
#include <ncurses.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <glib.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned short integralError;

static void printAuthor();
static void printHeading();
static void demonstration();

static GArray * integral_strerror(integralError error);
static void integral_error_free(GArray* errors);

static short call_fortran_function(const GArray *xVector, const GArray *yVector, const double from, const double to, double *result);
void integral_trapezoid_(const int *vecSize, const double xVector[], const double yVector[], const double *from, const double *to, double *result);

static double integral(const GArray *xVector, const GArray *yVector, const double from, const double to, unsigned short *error);



int main(void) {
  initscr();

  printHeading();

  menu:
  printw("Main menu:\n");
  printw("\t1) Demonstration.\n");
  printw("\t2) Calculate.\n");
  printw("\t3) Author.\n");
  printw("\t0) Exit.\n\n");
  printw("Enter menu item number: ");
  refresh();

  unsigned short menuItem = 0;

  scanw("%hu", &menuItem);
  printw("Your enter: %hu\n\n", menuItem);

  switch (menuItem) {
    case 0:
      goto exit;
    case 1:
      demonstration();
      break;
    case 2:
      demonstration();
      break;
    case 3:
      printHeading();
      printAuthor();
      break;
    default:
      printw("Wrong menu item. Try again.\n");
  }

  goto menu;

  exit:
  endwin();

  return 0;
}

void printAuthor() {
  printw("\nAuthor: Alexander Feodorov <afedorov@competentum.ru>\n");
  printw("\tSUAI group: z8432k\n\n");
}

void printHeading() {
  printw("Trapezoidal integration rule.\n");
  printw("Accurate for first degree polynomials.\n\n");
}

void demonstration() {
  #define DEMO_ARR_SIZE 10

  double xVector[DEMO_ARR_SIZE] = { 3,     3.5,  4,    4.5,  5,    5.5,  6,    6.5,  7,    7.5 };
  double yVector[DEMO_ARR_SIZE] = { 3.27,  3.53, 3.21, 2.34, 1.5,  1.27, 1.67, 2.18, 2.18, 1.53 };
  double from = 3.5;
  double to   = 7;

  size_t doubleSize = sizeof(double);

  GArray *xGVector = g_array_new(FALSE, FALSE, doubleSize);
  GArray *yGVector = g_array_new(FALSE, FALSE, doubleSize);

  printw("Demo:\n");

  short i;
  for (i = 0; i < DEMO_ARR_SIZE; i++) {
    g_array_append_val(xGVector, xVector[i]);
    g_array_append_val(yGVector, yVector[i]);
  }

  integralError error;
  double result = integral(xGVector, yGVector, from, to, &error);

  GArray *errorStrings = integral_strerror(error);

  if (result < 0) {
    printw("ERROR OCCURED: %s\n", strerror(errno));

    unsigned short i;
    for (i = 0; i < errorStrings->len; i++) {
      printw("\t- %s\n", g_array_index(errorStrings, char *, i));
    }
  }
  else {
    printw("Demo result: %f\n", result);
  }

  integral_error_free(errorStrings);
}

/*#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))*/

void integral_error_free(GArray* errors) {
  unsigned short i;
  for (i = 0; i < errors->len; i++) {
    free(g_array_index(errors, char *, i));
  }
}

GArray * integral_strerror(integralError error) {
  #define INTEGRAL_ERROR_MASK_SIZE 5

  GArray *errorStrings = g_array_new(FALSE, FALSE, sizeof(char *));
  integralError mask = 1;

  short i;
  char *str;
  for (i = 0; i < INTEGRAL_ERROR_MASK_SIZE; i++) {
    if (error & mask) {
      switch (mask) {
        case 1:
          str = "xVector size != yVector size";
          break;
        case 2:
          str = "from not in xVecto";
          break;
        case 4:
          str = "to not in xVector";
          break;
        case 8:
          str = "vectors too short";
        case 16:
          str = "from is >= to";
          break;
      }

      size_t strSize = strlen(str) + 1;
      char * buff = (char *) malloc(strSize);
      strncpy(buff, str, strSize);

      g_array_append_val(errorStrings, buff);
    }

    mask <<= 1;
  }

  return errorStrings;
}

double integral(const GArray *xVector, const GArray *yVector, const double from, const double to, integralError *error) {
  #define MIN_VECTOR_SIZE 2

  /**
   * Error flags
   * 01 [BITS: 00000001] - xVector size != yVector size
   * 02 [BITS: 00000010] - from not in xVector
   * 04 [BITS: 00000100] - to not in xVector
   * 08 [BITS: 00001000] - vectors too short
   * 16 [BITS: 00010000] - from is >= to
   *
   * 04 [BITS: 00001000] - Xn - Xn+1 = const
   */
  integralError errorFlags = 0;

  if (xVector->len != yVector->len) {
    errorFlags |= 1;
    goto has_error;
  }

  guint vecSize = xVector->len;

  if (vecSize < MIN_VECTOR_SIZE) {
    errorFlags |= 8;
    goto has_error;
  }

  if (from >= to) {
    errorFlags |= 16;
    goto has_error;
  }

  {
    /*
      Predefine errors
      from not in xVector
      to not in xVector
    */
    errorFlags = errorFlags | 6;

    guint i;
    /* double interval = 0; */
    for (i = 0; i < vecSize; i++) {
      double x = g_array_index(xVector, double, i);

      errorFlags ^= ((x == from) << 1);
      errorFlags ^= ((x == to) << 2);

      if (!(errorFlags & 6)) {
        goto calculation;
      }
    }

    goto has_error;
  }


  /* Calculation: */
  calculation:
  {
    double result = 0;
    short callResult = call_fortran_function(xVector, yVector, from, to, &result);

    /*
    guint index = 0;
    guint indexes = vecSize - 1;
    double result = 0;

    while (index < indexes) {
      guint nextIndex = index + 1;

      double x0 = g_array_index(xVector, double, index);
      double y0 = g_array_index(yVector, double, index);

      double x1 = g_array_index(xVector, double, nextIndex);
      double y1 = g_array_index(yVector, double, nextIndex);

      index++;

      if (x0 < from || x1 > to) {
        continue;
      }

      result += ((x1 - x0) * ((y0 + y1) / 2.0));
    }
    */

    return result;
  }


  has_error:

  errno = EINVAL;
  *error = errorFlags;

  return -1;
}

short call_fortran_function(const GArray *xVector, const GArray *yVector, const double from, const double to, double *result) {
  int vectorSize = (int) xVector->len;
  size_t doubleSize =  sizeof(double);

  double *f_xVector = (double *) calloc(doubleSize, xVector->len);
  double *f_yVector = (double *) calloc(doubleSize, yVector->len);

  int i;
  for (i = 0; i < vectorSize; i++) {
    f_xVector[i] = g_array_index(xVector, double, i);
    f_yVector[i] = g_array_index(yVector, double, i);
  }

  integral_trapezoid_(&vectorSize, f_xVector, f_yVector, &from, &to, result);

  free(f_xVector);
  free(f_yVector);

  return 0;
}
