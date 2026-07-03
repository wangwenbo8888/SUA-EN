/*
 * cos.c
 *
 * Code generation for function 'cos'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "cos.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void b_cos(real_T x_data[25], int32_T x_size[2])
{
  int32_T i13;
  int32_T k;
  i13 = x_size[1];
  for (k = 0; k <= i13 - 1; k++) {
    x_data[k] = cos(x_data[k]);
  }
}

void c_cos(emxArray_real_T *x)
{
  int32_T i16;
  int32_T k;
  i16 = x->size[0];
  for (k = 0; k <= i16 - 1; k++) {
    x->data[(int32_T)(1.0 + (real_T)k) - 1] = cos(x->data[(int32_T)(1.0 +
      (real_T)k) - 1]);
  }
}

/* End of code generation (cos.c) */
