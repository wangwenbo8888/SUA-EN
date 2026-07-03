/*
 * abs.c
 *
 * Code generation for function 'abs'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "abs.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static real_T rt_hypotd_snf(real_T u0, real_T u1);

/* Function Definitions */
static real_T rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T a;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) {
    a /= y;
    y *= sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * sqrt(y * y + 1.0);
  } else if (rtIsNaN(y)) {
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

real_T b_abs(const creal_T x)
{
  return rt_hypotd_snf(fabs(x.re), fabs(x.im));
}

void c_abs(const creal_T x_data[144], const int32_T x_size[1], real_T y_data[144],
           int32_T y_size[1])
{
  int32_T k;
  y_size[0] = 144;
  for (k = 0; k < 144; k++) {
    y_data[k] = rt_hypotd_snf(fabs(x_data[k].re), fabs(x_data[k].im));
  }
}

/* End of code generation (abs.c) */
