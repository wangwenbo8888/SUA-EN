/*
 * sin.c
 *
 * Code generation for function 'sin'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "sin.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void b_sin(real_T x_data[25], int32_T x_size[2])
{
  int32_T i14;
  int32_T k;
  i14 = x_size[1];
  for (k = 0; k <= i14 - 1; k++) {
    x_data[k] = sin(x_data[k]);
  }
}

void c_sin(emxArray_real_T *x)
{
  int32_T i15;
  int32_T k;
  i15 = x->size[0];
  for (k = 0; k <= i15 - 1; k++) {
    x->data[(int32_T)(1.0 + (real_T)k) - 1] = sin(x->data[(int32_T)(1.0 +
      (real_T)k) - 1]);
  }
}

/* End of code generation (sin.c) */
