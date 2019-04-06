#include <stdio.h>
#include <ncurses.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

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


double integral(double xVector[], double yVector[], double from, double to, unsigned short *error);

void demonstration() {
  printw("Demo:\n");

  double xVector[11] = { 3,     3.5,  4,    4.5,  5,    5.5,  6,    6.5,  7,    7.5, 0 };
  double yVector[10] = { 3.27,  3.53, 3.21, 2.34, 1.5,  1.27, 1.67, 2.18, 2.18, 1.53 };

  double from = 3.5;
  double to   = 7;

  unsigned short error;
  double result = integral(xVector, yVector, from, to, &error);

  if (result < 0) {
    printw("ERROR OCCURED: %s\n", strerror(errno));
  }
  else {
    printw("Demo result: %f\n", result);
  }
}

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

double integral(double xVector[], double yVector[], double from, double to, unsigned short *error) {
  /**
   * Error flags
   * 01 [BITS: 00000001] - xVector size != yVector size
   * 02 [BITS: 00000010] - from not in xVector
   * 04 [BITS: 00000100] - to not in xVector
   * 08 [BITS: 00001000] - vectors too short
   *
   * 04 [BITS: 00001000] - Xn - Xn+1 = const
   * 05 [BITS: 00010000] - from in array
   */
  unsigned short errorFlags = 0;

  size_t xVecSize = 10; /* error */
  size_t yVecSize = 10; /* error */
  size_t vecSize = xVecSize;

  if (xVecSize != yVecSize) {
    errorFlags = errorFlags | 1;
    goto has_error;
  }

  if (vecSize < 2) {
    errorFlags = errorFlags | 8;
    goto has_error;
  }

  {
    /*
      Predefine errors
      from not in xVector
      to not in xVector
    */
    errorFlags = errorFlags | 6;

    size_t i;
    /* double interval = 0; */
    for (i = 0; i < vecSize; i++) {
      double x = xVector[i];

      errorFlags = errorFlags ^ ((x == from) << 1);
      errorFlags = errorFlags ^ ((x == to) << 2);

      if (!(errorFlags & 6)) {
        goto from_to_is_ok;
      }
    }

    *error = errorFlags;
    goto has_error;
  }

  from_to_is_ok:

  /* Calculation: */
  {
    size_t steps = vecSize - 1;
    double result = 0;

    size_t i;

    for (i = 0; i < steps; i++) {
      double x0 = *xVector++;
      double y0 = *yVector++;

      result += (*xVector++ - x0) * ((y0 + *yVector++) / 2.0);
    }

    return result;
  }


  has_error:

  errno = EINVAL;
  *error = errorFlags;

  return -1;
}
