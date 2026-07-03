/*
 * abs.h
 *
 * Code generation for function 'abs'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __ABS_H__
#define __ABS_H__
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
extern real_T b_abs(const creal_T x);
extern void c_abs(const creal_T x_data[144], const int32_T x_size[1], real_T y_data[144], int32_T y_size[1]);
#ifdef __cplusplus
}
#endif
#endif
/* End of code generation (abs.h) */
