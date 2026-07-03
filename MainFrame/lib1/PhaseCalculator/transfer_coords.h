/*
 * transfer_coords.h
 *
 * Code generation for function 'transfer_coords'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __TRANSFER_COORDS_H__
#define __TRANSFER_COORDS_H__
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
extern void transfer_coords(const real_T center_coord[3], const real_T element_coords_data[1500], const int32_T element_coords_size[2], real_T coords_cartesian_data[1500], int32_T coords_cartesian_size[2]);
#ifdef __cplusplus
}
#endif
#endif
/* End of code generation (transfer_coords.h) */
