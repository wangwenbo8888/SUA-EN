/*
 * sin.h
 *
 * Code generation for function 'sin'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __SIN_H__
#define __SIN_H__
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
extern void b_sin(real_T x_data[25], int32_T x_size[2]);
extern void c_sin(emxArray_real_T *x);
#ifdef __cplusplus
}
#endif
#endif
/* End of code generation (sin.h) */
