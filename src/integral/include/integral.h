#include <errno.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include "integral_trapezoid.h"

typedef gushort integralError;

double integral(const GArray *xVector, const GArray *yVector, const double from, const double to, unsigned short *error);
GArray * integral_strerror(integralError error);
void integral_error_free(GArray* errors);
