#include <stdio.h>
#include <ncurses.h>
#include "include/demo.h"
#include "include/calculate.h"

static void printAuthor();
static void printHeading();

int main(void) {
  initscr();

  printHeading();

  menu:
  clear();
  printw("\nMain menu:\n");
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
      calculate();
      break;
    case 3:
      printHeading();
      printAuthor();
      break;
    default:
      printw("Wrong menu item. Try again.\n");
      goto menu;
  }

  printw("Press any key for return in main menu.\n");
  getch();

  goto menu;

  exit:
  endwin();

  return 0;
}

static void printAuthor() {
  printw("\nAuthor: Alexander Feodorov <afedorov@competentum.ru>\n");
  printw("\tSUAI group: z8432k\n\n");
}

static void printHeading() {
  printw("Trapezoidal integration rule.\n");
  printw("Accurate for first degree polynomials.\n\n");
}
