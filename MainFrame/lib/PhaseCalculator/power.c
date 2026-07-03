/*
 * power.c
 *
 * Code generation for function 'power'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "power.h"
#include "psuedo_inverse.h"
#include "PhaseCalculator_rtwutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void power(const real_T a_data[144], const int32_T a_size[1], real_T y_data[144],
           int32_T y_size[1])
{
  int32_T k;
  y_size[0] = 144;
  for (k = 0; k < 144; k++) {
    y_data[k] = rt_powd_snf(a_data[k], 2.0);
  }
}

/* End of code generation (power.c) */
