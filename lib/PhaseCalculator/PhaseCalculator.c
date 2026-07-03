/*
 * PhaseCalculator.c
 *
 * Code generation for function 'PhaseCalculator'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "PhaseCalculator_emxutil.h"
#include "psuedo_inverse.h"
#include "forward_matrix.h"
#include "divide_element.h"
#include "set_spot_params.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void PhaseCalculator(uint8_T focus_count, const real_T coords_focus[3], uint8_T
                     phase_data[144], int32_T phase_size[1])
{
  real_T b_coords_focus[3];
  int32_T i0;
  emxArray_creal_T *H;
  real_T expl_temp;
  real_T b_expl_temp;
  real_T spot_params_coords[3];
  uint8_T spot_params_focus_count;
  int32_T delta_area_size[1];
  real_T delta_area_data[500];
  int32_T element_coords_size[2];
  real_T element_coords_data[1500];

  /*  comment when generating the C codes */
  /*  format long; */
  for (i0 = 0; i0 < 3; i0++) {
    b_coords_focus[i0] = coords_focus[i0];
  }

  emxInit_creal_T(&H, 2);
  set_spot_params(focus_count, b_coords_focus, &spot_params_focus_count,
                  spot_params_coords, &b_expl_temp, &expl_temp);
  divide_element(0.005, 20.0, 20.0, element_coords_data, element_coords_size,
                 delta_area_data, delta_area_size);
  forward_matrix(element_coords_data, element_coords_size, delta_area_data,
                 delta_area_size, spot_params_focus_count, spot_params_coords, H);
  psuedo_inverse(H, spot_params_focus_count, 1.0904850535426885E+6, phase_data,
                 phase_size);
  emxFree_creal_T(&H);
}

/* End of code generation (PhaseCalculator.c) */
