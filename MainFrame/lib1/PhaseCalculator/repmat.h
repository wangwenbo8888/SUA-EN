/*
 * repmat.h
 *
 * Code generation for function 'repmat'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __REPMAT_H__
#define __REPMAT_H__
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
extern void b_repmat(const real_T a[3], real_T m, real_T b_data[1500], int32_T b_size[2]);
extern void c_repmat(real_T a, uint8_T m, real_T b_data[255], int32_T b_size[1]);
extern void repmat(const real_T a[3], real_T n, real_T b_data[1500], int32_T b_size[2]);
#ifdef __cplusplus
}
#endif
#endif
/* End of code generation (repmat.h) */
