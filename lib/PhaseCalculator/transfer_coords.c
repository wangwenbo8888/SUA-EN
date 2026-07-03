/*
 * transfer_coords.c
 *
 * Code generation for function 'transfer_coords'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "transfer_coords.h"
#include "repmat.h"
#include "psuedo_inverse.h"
#include "PhaseCalculator_rtwutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static void eml_xgemm(int32_T n, const real_T A[9], const real_T B_data[1500],
                      const int32_T B_size[2], real_T C_data[1500], int32_T
                      C_size[2]);

/* Function Definitions */
static void eml_xgemm(int32_T n, const real_T A[9], const real_T B_data[1500],
                      const int32_T B_size[2], real_T C_data[1500], int32_T
                      C_size[2])
{
  int32_T c;
  int32_T cr;
  int32_T ic;
  int32_T br;
  int32_T ar;
  int32_T ib;
  int32_T ia;
  if (n == 0) {
  } else {
    c = 3 * (n - 1);
    for (cr = 0; cr <= c; cr += 3) {
      for (ic = cr; ic + 1 <= cr + 3; ic++) {
        C_data[ic] = 0.0;
      }
    }

    br = 0;
    for (cr = 0; cr <= c; cr += 3) {
      ar = -1;
      for (ib = br; ib + 1 <= br + 3; ib++) {
        if (B_data[ib] != 0.0) {
          ia = ar;
          for (ic = cr; ic + 1 <= cr + 3; ic++) {
            ia++;
            C_data[ic] += B_data[ib] * A[ia];
          }
        }

        ar += 3;
      }

      br += 3;
    }
  }
}

void transfer_coords(const real_T center_coord[3], const real_T
                     element_coords_data[1500], const int32_T
                     element_coords_size[2], real_T coords_cartesian_data[1500],
                     int32_T coords_cartesian_size[2])
{
  real_T theta;
  real_T r;
  int32_T translation_size[2];
  int32_T loop_ub;
  int32_T i3;
  int32_T i4;
  real_T translation_data[1500];
  real_T coords_data[1500];
  int32_T coords_size[2];
  real_T dv2[9];
  real_T dv3[9];
  static const int8_T iv1[3] = { 0, 0, 1 };

  static const int8_T iv2[3] = { 0, 1, 0 };

  real_T dv4[9];
  real_T dv5[9];
  static const int8_T b[9] = { 0, -1, 0, 1, 0, 0, 0, 0, 1 };

  /*     */
  /*    Angle between z-axis and negative of center */
  theta = acos(-center_coord[2] / sqrt((rt_powd_snf(center_coord[0], 2.0) +
    rt_powd_snf(center_coord[1], 2.0)) + rt_powd_snf(center_coord[2], 2.0)));
  r = sqrt(rt_powd_snf(center_coord[0], 2.0) + rt_powd_snf(center_coord[1], 2.0));
  if (r < 1.0E-6) {
    r = 1.5707963267948966;
  } else {
    r = acos(-center_coord[0] / r);
  }

  if (center_coord[1] > 0.0) {
    r = 6.2831853071795862 - r;
  }

  translation_size[0] = 3;
  translation_size[1] = element_coords_size[0];
  loop_ub = element_coords_size[0] - 1;
  for (i3 = 0; i3 <= loop_ub; i3++) {
    for (i4 = 0; i4 < 3; i4++) {
      translation_data[i4 + 3 * i3] = element_coords_data[i3 +
        element_coords_size[0] * i4];
    }
  }

  coords_size[0] = 3;
  coords_size[1] = (int16_T)translation_size[1];
  coords_size[0] = 3;
  loop_ub = coords_size[1] - 1;
  for (i3 = 0; i3 <= loop_ub; i3++) {
    for (i4 = 0; i4 < 3; i4++) {
      coords_data[i4 + 3 * i3] = 0.0;
    }
  }

  dv2[0] = cos(r);
  dv2[3] = -sin(r);
  dv2[6] = 0.0;
  dv2[1] = sin(r);
  dv2[4] = cos(r);
  dv2[7] = 0.0;
  dv3[0] = cos(theta);
  dv3[3] = 0.0;
  dv3[6] = sin(theta);
  for (i3 = 0; i3 < 3; i3++) {
    dv2[2 + 3 * i3] = (real_T)iv1[i3];
    dv3[1 + 3 * i3] = (real_T)iv2[i3];
  }

  dv3[2] = -sin(theta);
  dv3[5] = 0.0;
  dv3[8] = cos(theta);
  for (i3 = 0; i3 < 3; i3++) {
    for (i4 = 0; i4 < 3; i4++) {
      dv4[i3 + 3 * i4] = 0.0;
      for (loop_ub = 0; loop_ub < 3; loop_ub++) {
        dv4[i3 + 3 * i4] += dv2[i3 + 3 * loop_ub] * dv3[loop_ub + 3 * i4];
      }
    }

    for (i4 = 0; i4 < 3; i4++) {
      dv5[i3 + 3 * i4] = 0.0;
      for (loop_ub = 0; loop_ub < 3; loop_ub++) {
        dv5[i3 + 3 * i4] += dv4[i3 + 3 * loop_ub] * (real_T)b[loop_ub + 3 * i4];
      }
    }
  }

  eml_xgemm(translation_size[1], dv5, translation_data, translation_size,
            coords_data, coords_size);
  repmat(center_coord, (real_T)coords_size[1], translation_data,
         translation_size);
  coords_cartesian_size[0] = coords_size[1];
  coords_cartesian_size[1] = 3;
  for (i3 = 0; i3 < 3; i3++) {
    loop_ub = coords_size[1] - 1;
    for (i4 = 0; i4 <= loop_ub; i4++) {
      coords_cartesian_data[i4 + coords_cartesian_size[0] * i3] = coords_data[i3
        + coords_size[0] * i4] + translation_data[i3 + translation_size[0] * i4];
    }
  }
}

/* End of code generation (transfer_coords.c) */
