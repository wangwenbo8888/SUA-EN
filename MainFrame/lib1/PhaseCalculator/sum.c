/*
 * sum.c
 *
 * Code generation for function 'sum'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "sum.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void b_sum(const real_T x_data[1500], const int32_T x_size[2], real_T y_data[500],
           int32_T y_size[1])
{
  int16_T sz[2];
  int32_T iy;
  int32_T ixstart;
  int32_T j;
  int32_T ix;
  real_T s;
  int32_T k;
  for (iy = 0; iy < 2; iy++) {
    sz[iy] = (int16_T)x_size[iy];
  }

  sz[1] = 1;
  y_size[0] = sz[0];
  if (x_size[0] == 0) {
    iy = y_size[0];
    ixstart = iy - 1;
    for (iy = 0; iy <= ixstart; iy++) {
      y_data[iy] = 0.0;
    }
  } else {
    iy = -1;
    ixstart = -1;
    for (j = 1; j <= x_size[0]; j++) {
      ixstart++;
      ix = ixstart;
      s = x_data[ixstart];
      for (k = 0; k < 2; k++) {
        ix += x_size[0];
        s += x_data[ix];
      }

      iy++;
      y_data[iy] = s;
    }
  }
}

real_T sum(const real_T x_data[144], const int32_T x_size[1])
{
  real_T y;
  int32_T k;
  if (x_size[0] == 0) {
    y = 0.0;
  } else {
    y = x_data[0];
    for (k = 2; k <= x_size[0]; k++) {
      y += x_data[k - 1];
    }
  }

  return y;
}

/* End of code generation (sum.c) */
