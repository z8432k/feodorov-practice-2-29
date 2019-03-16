#include <stdio.h>

void fortran_function_();

int main(void) {
  unsigned char a1 = 1;
  short a2 = 22;
  int a3 = -333;
  long a4 = 4444;

  fortran_function_(&a1, &a2, &a3, &a4);
  printf("trapezoid method.\n");
  return 0;
}
