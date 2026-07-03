/*
 * round.c
 *
 * Code generation for function 'round'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "round.h"
#include "psuedo_inverse.h"
#include "PhaseCalculator_rtwutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void b_round(real_T x_data[144], int32_T x_size[1])
{
  int32_T k;
  for (k = 0; k < 144; k++) {
    x_data[k] = rt_roundd_snf(x_data[k]);
  }
}

/* End of code generation (round.c) */
