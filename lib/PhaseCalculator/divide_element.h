/*
 * divide_element.h
 *
 * Code generation for function 'divide_element'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __DIVIDE_ELEMENT_H__
#define __DIVIDE_ELEMENT_H__
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
extern void divide_element(real_T element_radius, real_T count_in_radius, real_T count_in_angle, real_T element_coords_data[1500], int32_T element_coords_size[2], real_T delta_area_data[500], int32_T delta_area_size[1]);
#ifdef __cplusplus
}
#endif
#endif
/* End of code generation (divide_element.h) */
