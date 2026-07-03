/*
 * angle.c
 *
 * Code generation for function 'angle'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "angle.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static real_T rt_atan2d_snf(real_T u0, real_T u1);

/* Function Definitions */
static real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T i11;
  int32_T i12;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u1 > 0.0) {
      i11 = 1;
    } else {
      i11 = -1;
    }

    if (u0 > 0.0) {
      i12 = 1;
    } else {
      i12 = -1;
    }

    y = atan2((real_T)i12, (real_T)i11);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

void angle(const creal_T x_data[144], const int32_T x_size[1], real_T y_data[144],
           int32_T y_size[1])
{
  int32_T k;
  y_size[0] = 144;
  for (k = 0; k < 144; k++) {
    y_data[k] = 0.0;
    y_data[k] = rt_atan2d_snf(x_data[k].im, x_data[k].re);
  }
}

/* End of code generation (angle.c) */
