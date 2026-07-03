/*
 * set_spot_params.c
 *
 * Code generation for function 'set_spot_params'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "set_spot_params.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void set_spot_params(uint8_T focus_count, real_T coords[3], uint8_T
                     *spot_params_focus_count, real_T spot_params_coords[3],
                     real_T *spot_params_intensity, real_T *spot_params_pressure)
{
  int32_T i1;

  /*    2016/03/02 */
  /*    JI Xiang */
  /*    Intensity at the focal spot: W/m^3 */
  /*    Coordinates of the focal spot -> vector has three elements (x, y, z): m */
  coords[2] = 0.14 - coords[2];

  /*    Pressure at the focal spot: Pa */
  *spot_params_focus_count = focus_count;
  for (i1 = 0; i1 < 3; i1++) {
    spot_params_coords[i1] = coords[i1];
  }

  *spot_params_intensity = 333000.0;
  *spot_params_pressure = 1.0904850535426885E+6;
}

/* End of code generation (set_spot_params.c) */
