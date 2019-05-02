#include <stdio.h>
#include <ncurses.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <glib.h>

void fortran_function_(unsigned char *a1, short *a2, int *a3, long *a4);
static void printAuthor();
static void printHeading();
static void demonstration();

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


double integral(GArray *xVector, GArray *yVector, double from, double to, unsigned short *error);

void demonstration() {
  #define DEMO_ARR_SIZE 10

  double xVector[DEMO_ARR_SIZE] = { 3,     3.5,  4,    4.5,  5,    5.5,  6,    6.5,  7,    7.5 };
  double yVector[DEMO_ARR_SIZE] = { 3.27,  3.53, 3.21, 2.34, 1.5,  1.27, 1.67, 2.18, 2.18, 1.53, };

  size_t doubleSize = sizeof(double);

  GArray *xGVector = g_array_new(FALSE, FALSE, doubleSize);
  GArray *yGVector = g_array_new(FALSE, FALSE, doubleSize);


  printw("Demo:\n");

  short i;
  for (i = 0; i < DEMO_ARR_SIZE; i++) {
    g_array_append_val(xGVector, xVector[i]);
    g_array_append_val(yGVector, yVector[i]);
  }

  double from = 3.5;
  double to   = 7;

  unsigned short error;
  double result = integral(xGVector, yGVector, from, to, &error);

  if (result < 0) {
    printw("ERROR OCCURED: %s\n", strerror(errno));
  }
  else {
    printw("Demo result: %f\n", result);
  }
}

/*#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))*/

double integral(GArray *xVector, GArray *yVector, double from, double to, unsigned short *error) {
  #define MIN_VECTOR_SIZE 2

  /**
   * Error flags
   * 01 [BITS: 00000001] - xVector size != yVector size
   * 02 [BITS: 00000010] - from not in xVector
   * 04 [BITS: 00000100] - to not in xVector
   * 08 [BITS: 00001000] - vectors too short
   *
   * 04 [BITS: 00001000] - Xn - Xn+1 = const
   */
  unsigned short errorFlags = 0;

  if (xVector->len != yVector->len) {
    errorFlags |= 1;
  }

  guint vecSize = xVector->len;

  if (vecSize < MIN_VECTOR_SIZE) {
    errorFlags |= 8;
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
        goto from_to_is_ok;
      }
    }

    goto has_error;
  }

  from_to_is_ok:

  /* Calculation: */
  {
    guint index = 0;
    guint indexes = vecSize - 1;
    double result = 0;

    while (index < indexes) {
      guint nextIndex = index + 1;

      double x0 = g_array_index(xVector, double, index);
      double y0 = g_array_index(yVector, double, index);

      double x1 = g_array_index(xVector, double, nextIndex);
      double y1 = g_array_index(yVector, double, nextIndex);

      result += ((x1 - x0) * ((y0 + y1) / 2.0));

      index++;
    }

    return result;
  }


  has_error:

  errno = EINVAL;
  *error = errorFlags;

  return -1;
}
