#include <stdio.h>
#include <ncurses.h>

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

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
double integral(double data[10][2], double *interval);

double integral(double data[10][2], double *interval) {
  size_t xPos = 0;
  size_t yPos = 1;
  size_t dataSize = 10;
  size_t steps = dataSize - 1;
  double result = 0;

  int i;
  double x0;
  double y0;
  double x;
  double y;
  double *currentPoint;
  double *nextPoint;
  double currentResutl = 0;

  for (i = 0; i < steps; i++) {
    currentPoint = data[i];
    x0 = currentPoint[xPos];
    y0 = currentPoint[yPos];

    i++;

    nextPoint = data[i];
    x = nextPoint[xPos];
    y = nextPoint[yPos];

    currentResutl = (x - x0) * ((y0 + y) / 2.0);
    result += currentResutl;

    currentPoint = nextPoint;
  }

  return result;
}


void demonstration() {
  printw("Demo:\n");

  double data [10][2] = {
    {3, 3.27},    {3.5, 3.53},  {4, 3.21},    {4.5, 2.34},  {5, 1.5},
    {5.5, 1.27},  {6, 1.67},    {6.5, 2.18},  {7, 2.18},    {7.5, 1.53}
  };

  double interval [2] = {3.5, 7};

  integral(data, interval);

  printw("Item: %f\n", data[0][0]);
}
