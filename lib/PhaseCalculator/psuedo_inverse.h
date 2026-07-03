/*
 * psuedo_inverse.h
 *
 * Code generation for function 'psuedo_inverse'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __PSUEDO_INVERSE_H__
#define __PSUEDO_INVERSE_H__
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
extern void psuedo_inverse(const emxArray_creal_T *H, uint8_T focus_count, real_T pressure, uint8_T phase_data[144], int32_T phase_size[1]);
#ifdef __cplusplus
}
#endif
#endif
/* End of code generation (psuedo_inverse.h) */
