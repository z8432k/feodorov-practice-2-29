#include "include/integral.h"

static void fortran_trapezoid(const GArray *xVector, const GArray *yVector, const double from, const double to, double *result);

static char *err1 = "xVector size != yVector size";

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
          str = err1;
          break;
        case 2:
          str = "from not in xVector";
          break;
        case 4:
          str = "to not in xVector";
          break;
        case 8:
          str = "vectors too short";
          break;
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
    fortran_trapezoid(xVector, yVector, from, to, &result);

    return result;
  }


  has_error:

  errno = EINVAL;
  *error = errorFlags;

  return -1;
}

static void fortran_trapezoid(const GArray *xVector, const GArray *yVector, const double from, const double to, double *result) {
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
}
