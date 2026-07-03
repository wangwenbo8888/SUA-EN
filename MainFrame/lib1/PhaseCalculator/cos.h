/*
 * cos.h
 *
 * Code generation for function 'cos'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __COS_H__
#define __COS_H__
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
extern void b_cos(real_T x_data[25], int32_T x_size[2]);
extern void c_cos(emxArray_real_T *x);
#ifdef __cplusplus
}
#endif
#endif
/* End of code generation (cos.h) */
