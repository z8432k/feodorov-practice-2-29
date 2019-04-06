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

void demonstration() {
  printw("Demo:\n");

  double data [10][2] = {
    {3, 3.27},    {3.5, 3.53},  {4, 3.21},    {4.5, 2.34},  {5, 1.5},
    {5.5, 1.27},  {6, 1.67},    {6.5, 2.18},  {7, 2.18},    {7.5, 1.53}
  };

  printw("Item: %f\n", data[0][0]);
}
