/*
 * sum.h
 *
 * Code generation for function 'sum'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __SUM_H__
#define __SUM_H__
/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"

#include "rtwtypes.h"
#include "PhaseCalculator_types.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
#ifdef __cplusplus
extern "C" {
#endif
extern void b_sum(const real_T x_data[1500], const int32_T x_size[2], real_T y_data[500], int32_T y_size[1]);
extern real_T sum(const real_T x_data[144], const int32_T x_size[1]);
#ifdef __cplusplus
}
#endif
#endif
/* End of code generation (sum.h) */
