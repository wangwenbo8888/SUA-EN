/*
 * divide_element.c
 *
 * Code generation for function 'divide_element'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "divide_element.h"
#include "psuedo_inverse.h"
#include "sum.h"
#include "sin.h"
#include "cos.h"
#include "colon.h"
#include "PhaseCalculator_rtwutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void divide_element(real_T element_radius, real_T count_in_radius, real_T
                    count_in_angle, real_T element_coords_data[1500], int32_T
                    element_coords_size[2], real_T delta_area_data[500], int32_T
                    delta_area_size[1])
{
  boolean_T n_too_large;
  real_T bnew;
  real_T angle_index;
  int32_T n;
  real_T y_data[24];
  int32_T nm1;
  int32_T nm1d2;
  int32_T k;
  real_T kd;
  int32_T b_n;
  real_T angles_data[25];
  int32_T angles_size[2];
  int32_T b_size[2];
  int32_T i2;
  real_T b_data[25];
  real_T b_y_data[20];
  int32_T element_coords_x_size[2];
  real_T element_coords_x_data[500];
  real_T element_coords_y_data[500];
  int8_T iv0[2];
  real_T b_delta_area_data[500];
  real_T c_delta_area_data[144];
  int32_T b_delta_area_size[1];
  real_T b_element_coords_x_data[500];
  real_T tmp_data[400];
  real_T b_tmp_data[400];

  /*    2016/03/02 */
  /*    JI Xiang */
  float_colon_length(0.000125, 0.00025, 0.004875, &n, &angle_index, &bnew,
                     &n_too_large);
  if (n > 0) {
    y_data[0] = angle_index;
    if (n > 1) {
      y_data[n - 1] = bnew;
      nm1 = n - 1;
      nm1d2 = nm1 / 2;
      for (k = 1; k <= nm1d2 - 1; k++) {
        kd = (real_T)k * 0.00025;
        y_data[k] = angle_index + kd;
        y_data[(n - k) - 1] = bnew - kd;
      }

      if (nm1d2 << 1 == nm1) {
        y_data[nm1d2] = (angle_index + bnew) / 2.0;
      } else {
        kd = (real_T)nm1d2 * 0.00025;
        y_data[nm1d2] = angle_index + kd;
        y_data[nm1d2 + 1] = bnew - kd;
      }
    }
  }

  float_colon_length(0.31415926535897931, 0.31415926535897931,
                     6.2831853071795862, &b_n, &angle_index, &bnew, &n_too_large);
  angles_size[0] = 1;
  angles_size[1] = b_n;
  if (b_n > 0) {
    angles_data[0] = angle_index;
    if (b_n > 1) {
      angles_data[b_n - 1] = bnew;
      nm1 = b_n - 1;
      nm1d2 = nm1 / 2;
      for (k = 1; k <= nm1d2 - 1; k++) {
        kd = (real_T)k * 0.31415926535897931;
        angles_data[k] = angle_index + kd;
        angles_data[(b_n - k) - 1] = bnew - kd;
      }

      if (nm1d2 << 1 == nm1) {
        angles_data[nm1d2] = (angle_index + bnew) / 2.0;
      } else {
        kd = (real_T)nm1d2 * 0.31415926535897931;
        angles_data[nm1d2] = angle_index + kd;
        angles_data[nm1d2 + 1] = bnew - kd;
      }
    }
  }

  b_size[0] = 1;
  b_size[1] = angles_size[1];
  b_n = angles_size[1] - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    b_data[i2] = angles_data[i2];
  }

  b_cos(b_data, b_size);
  b_n = n - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    b_y_data[i2] = y_data[i2];
  }

  element_coords_x_size[0] = n;
  element_coords_x_size[1] = b_size[1];
  b_n = b_size[1] - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    nm1 = n - 1;
    for (nm1d2 = 0; nm1d2 <= nm1; nm1d2++) {
      element_coords_x_data[nm1d2 + element_coords_x_size[0] * i2] =
        b_y_data[nm1d2] * b_data[b_size[0] * i2];
    }
  }

  b_sin(angles_data, angles_size);
  b_n = n - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    b_y_data[i2] = y_data[i2];
  }

  b_n = angles_size[1] - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    nm1 = n - 1;
    for (nm1d2 = 0; nm1d2 <= nm1; nm1d2++) {
      element_coords_y_data[nm1d2 + n * i2] = b_y_data[nm1d2] *
        angles_data[angles_size[0] * i2];
    }
  }

  for (i2 = 0; i2 < 2; i2++) {
    iv0[i2] = (int8_T)element_coords_x_size[i2];
  }

  b_n = iv0[0] * iv0[1] - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    b_delta_area_data[i2] = 0.0;
  }

  angle_index = 0.0;

  /*  comment when generating the C codes */
  /*  for angle = angles */
  for (nm1 = 0; nm1 < 20; nm1++) {
    angle_index++;
    nm1d2 = -1;

    /*  comment when generating the C codes */
    /*      for radius = radii */
    for (k = 0; k < 20; k++) {
      bnew = 0.000125 + (real_T)k * 0.00025;
      nm1d2++;
      if (nm1d2 + 1 == 1) {
        b_delta_area_data[iv0[0] * ((int32_T)angle_index - 1)] = rt_powd_snf
          (bnew, 2.0) * 3.1415926535897931 / 20.0;
      } else {
        b_delta_area_size[0] = iv0[0];
        b_n = iv0[0] - 1;
        for (i2 = 0; i2 <= b_n; i2++) {
          c_delta_area_data[i2] = b_delta_area_data[i2 + iv0[0] * ((int32_T)
            angle_index - 1)];
        }

        b_delta_area_data[nm1d2 + iv0[0] * ((int32_T)angle_index - 1)] =
          rt_powd_snf(bnew, 2.0) * 3.1415926535897931 / 20.0 - sum
          (c_delta_area_data, b_delta_area_size);
      }
    }
  }

  delta_area_size[0] = iv0[0] * iv0[1];
  b_n = iv0[0] * iv0[1] - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    delta_area_data[i2] = b_delta_area_data[i2];
  }

  nm1 = element_coords_x_size[0] * element_coords_x_size[1];
  k = element_coords_x_size[0] * element_coords_x_size[1];
  b_n = nm1 - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    b_element_coords_x_data[i2] = element_coords_x_data[i2];
  }

  nm1 = n * angles_size[1];
  nm1d2 = n * angles_size[1];
  b_n = nm1 - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    b_delta_area_data[i2] = element_coords_y_data[i2];
  }

  nm1 = n * angles_size[1];
  i2 = 0;
  while (i2 <= 0) {
    b_n = nm1d2 - 1;
    for (i2 = 0; i2 <= b_n; i2++) {
      element_coords_y_data[i2] = b_delta_area_data[i2];
    }

    i2 = 1;
  }

  for (i2 = 0; i2 < 20; i2++) {
    memset(&tmp_data[20 * i2], 0, 20U * sizeof(real_T));
  }

  memcpy(&b_tmp_data[0], &tmp_data[0], 400U * sizeof(real_T));
  element_coords_size[0] = k;
  element_coords_size[1] = 3;
  i2 = 0;
  while (i2 <= 0) {
    b_n = k - 1;
    for (i2 = 0; i2 <= b_n; i2++) {
      element_coords_data[i2] = b_element_coords_x_data[i2];
    }

    i2 = 1;
  }

  b_n = nm1 - 1;
  for (i2 = 0; i2 <= b_n; i2++) {
    element_coords_data[i2 + element_coords_size[0]] = element_coords_y_data[i2];
  }

  memcpy(&element_coords_data[element_coords_size[0] * 2], &b_tmp_data[0], 400U *
         sizeof(real_T));
}

/* End of code generation (divide_element.c) */
