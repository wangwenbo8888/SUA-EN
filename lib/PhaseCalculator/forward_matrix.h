/*
 * forward_matrix.h
 *
 * Code generation for function 'forward_matrix'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __FORWARD_MATRIX_H__
#define __FORWARD_MATRIX_H__
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
extern void forward_matrix(const real_T element_coords_data[1500], const int32_T element_coords_size[2], const real_T delta_area_data[500], const int32_T delta_area_size[1], uint8_T spot_params_focus_count, const real_T spot_params_coords[3], emxArray_creal_T *H);
#ifdef __cplusplus
}
#endif
#endif
/* End of code generation (forward_matrix.h) */
