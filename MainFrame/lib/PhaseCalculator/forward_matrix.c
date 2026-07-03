/*
 * forward_matrix.c
 *
 * Code generation for function 'forward_matrix'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "forward_matrix.h"
#include "psuedo_inverse.h"
#include "sum.h"
#include "repmat.h"
#include "PhaseCalculator_emxutil.h"
#include "transfer_coords.h"
#include "sin.h"
#include "cos.h"
#include "set_array_params.h"
#include "PhaseCalculator_rtwutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static void eml_scalexp_alloc(const real_T varargin_1_data[1500], const int32_T
  varargin_1_size[2], real_T z_data[1500], int32_T z_size[2]);

/* Function Definitions */
static void eml_scalexp_alloc(const real_T varargin_1_data[1500], const int32_T
  varargin_1_size[2], real_T z_data[1500], int32_T z_size[2])
{
  int16_T iv3[2];
  int32_T i5;
  for (i5 = 0; i5 < 2; i5++) {
    iv3[i5] = (int16_T)varargin_1_size[i5];
  }

  z_size[0] = iv3[0];
  z_size[1] = 3;
}

void forward_matrix(const real_T element_coords_data[1500], const int32_T
                    element_coords_size[2], const real_T delta_area_data[500],
                    const int32_T delta_area_size[1], uint8_T
                    spot_params_focus_count, const real_T spot_params_coords[3],
                    emxArray_creal_T *H)
{
  emxArray_real_T *array_params_angles_with_propa;
  emxArray_real_T *c_array_params_angles_vertical_;
  real_T expl_temp[6];
  real_T b_expl_temp[6];
  real_T c_expl_temp;
  real_T d_expl_temp[6];
  real_T e_expl_temp;
  real_T f_expl_temp;
  real_T g_expl_temp;
  real_T s_im;
  real_T s_re;
  int32_T ib;
  int32_T ia;
  real_T coords_data[765];
  emxArray_real_T *center_coords;
  emxArray_real_T *b;
  emxArray_real_T *r0;
  int32_T k;
  int32_T exitg1;
  emxArray_real_T *r;
  emxArray_creal_T *temp;
  emxArray_real_T *r1;
  emxArray_creal_T *y;
  int32_T b_index;
  real_T b_center_coords[3];
  int32_T coords_cartesian_size[2];
  real_T coords_cartesian_data[1500];
  uint8_T count;
  int32_T a_size[2];
  real_T a_data[1500];
  int32_T y_size[2];
  real_T y_data[1500];
  int32_T x_size[1];
  real_T x_data[500];
  real_T tmp_data[500];
  uint8_T mv[2];
  int16_T delta_area[2];
  int16_T outsize[2];
  int32_T vlen;
  int16_T sz[2];
  creal_T b_y_data[254];
  int32_T ix;
  int32_T iy;
  int32_T i;
  creal_T H_data[255];
  emxInit_real_T(&array_params_angles_with_propa, 2);
  emxInit_real_T(&c_array_params_angles_vertical_, 2);

  /*    2016/03/02 */
  /*    JI Xiang */
  set_array_params(&s_re, &s_im, &g_expl_temp, &f_expl_temp, &e_expl_temp,
                   d_expl_temp, &c_expl_temp, b_expl_temp, expl_temp,
                   array_params_angles_with_propa,
                   c_array_params_angles_vertical_);
  ib = spot_params_focus_count * 3 - 1;
  for (ia = 0; ia <= ib; ia++) {
    coords_data[ia] = 0.0;
  }

  for (ia = 0; ia < 3; ia++) {
    coords_data[spot_params_focus_count * ia] = spot_params_coords[ia];
  }

  if (spot_params_focus_count == 2) {
    for (ia = 0; ia < 3; ia++) {
      coords_data[1 + 2 * ia] = -spot_params_coords[ia];
    }
  }

  emxInit_real_T(&center_coords, 2);
  ib = c_array_params_angles_vertical_->size[1];
  ia = center_coords->size[0] * center_coords->size[1];
  center_coords->size[0] = ib;
  center_coords->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)center_coords, ia, (int32_T)sizeof
                    (real_T));
  ib = c_array_params_angles_vertical_->size[1] * 3 - 1;
  for (ia = 0; ia <= ib; ia++) {
    center_coords->data[ia] = 0.0;
  }

  b_emxInit_real_T(&b, 1);
  ia = b->size[0];
  b->size[0] = array_params_angles_with_propa->size[1];
  emxEnsureCapacity((emxArray__common *)b, ia, (int32_T)sizeof(real_T));
  ib = array_params_angles_with_propa->size[1] - 1;
  for (ia = 0; ia <= ib; ia++) {
    b->data[ia] = array_params_angles_with_propa->data[ia];
  }

  b_emxInit_real_T(&r0, 1);
  c_sin(b);
  ia = r0->size[0];
  r0->size[0] = c_array_params_angles_vertical_->size[1];
  emxEnsureCapacity((emxArray__common *)r0, ia, (int32_T)sizeof(real_T));
  ib = c_array_params_angles_vertical_->size[1] - 1;
  for (ia = 0; ia <= ib; ia++) {
    r0->data[ia] = c_array_params_angles_vertical_->data[ia];
  }

  c_cos(r0);
  ib = b->size[0] - 1;
  for (ia = 0; ia <= ib; ia++) {
    center_coords->data[ia] = 0.14 * b->data[ia] * r0->data[ia];
  }

  ia = b->size[0];
  b->size[0] = array_params_angles_with_propa->size[1];
  emxEnsureCapacity((emxArray__common *)b, ia, (int32_T)sizeof(real_T));
  ib = array_params_angles_with_propa->size[1] - 1;
  for (ia = 0; ia <= ib; ia++) {
    b->data[ia] = array_params_angles_with_propa->data[ia];
  }

  c_sin(b);
  ia = r0->size[0];
  r0->size[0] = c_array_params_angles_vertical_->size[1];
  emxEnsureCapacity((emxArray__common *)r0, ia, (int32_T)sizeof(real_T));
  ib = c_array_params_angles_vertical_->size[1] - 1;
  for (ia = 0; ia <= ib; ia++) {
    r0->data[ia] = c_array_params_angles_vertical_->data[ia];
  }

  emxFree_real_T(&c_array_params_angles_vertical_);
  c_sin(r0);
  ib = b->size[0] - 1;
  for (ia = 0; ia <= ib; ia++) {
    center_coords->data[ia + center_coords->size[0]] = 0.14 * b->data[ia] *
      r0->data[ia];
  }

  emxFree_real_T(&r0);
  ia = b->size[0];
  b->size[0] = array_params_angles_with_propa->size[1];
  emxEnsureCapacity((emxArray__common *)b, ia, (int32_T)sizeof(real_T));
  ib = array_params_angles_with_propa->size[1] - 1;
  for (ia = 0; ia <= ib; ia++) {
    b->data[ia] = array_params_angles_with_propa->data[ia];
  }

  k = 0;
  do {
    exitg1 = 0;
    ib = array_params_angles_with_propa->size[1];
    if (k <= ib - 1) {
      b->data[k] = cos(b->data[k]);
      k++;
    } else {
      exitg1 = 1;
    }
  } while (exitg1 == 0U);

  emxFree_real_T(&array_params_angles_with_propa);
  ib = b->size[0] - 1;
  for (ia = 0; ia <= ib; ia++) {
    center_coords->data[ia + (center_coords->size[0] << 1)] = 0.14 * b->data[ia];
  }

  emxFree_real_T(&b);
  ia = H->size[0] * H->size[1];
  H->size[0] = spot_params_focus_count;
  H->size[1] = 144;
  emxEnsureCapacity((emxArray__common *)H, ia, (int32_T)sizeof(creal_T));
  ib = spot_params_focus_count * 144 - 1;
  for (ia = 0; ia <= ib; ia++) {
    H->data[ia].re = 0.0;
    H->data[ia].im = 1.54768E+9;
  }

  emxInit_real_T(&r, 2);
  emxInit_creal_T(&temp, 2);
  emxInit_real_T(&r1, 2);
  emxInit_creal_T(&y, 2);
  for (b_index = 0; b_index < 144; b_index++) {
    for (ia = 0; ia < 3; ia++) {
      b_center_coords[ia] = center_coords->data[b_index + center_coords->size[0]
        * ia];
    }

    transfer_coords(b_center_coords, element_coords_data, element_coords_size,
                    coords_cartesian_data, coords_cartesian_size);
    ia = r->size[0] * r->size[1];
    r->size[0] = coords_cartesian_size[0];
    emxEnsureCapacity((emxArray__common *)r, ia, (int32_T)sizeof(real_T));
    ia = r->size[0] * r->size[1];
    r->size[1] = spot_params_focus_count;
    emxEnsureCapacity((emxArray__common *)r, ia, (int32_T)sizeof(real_T));
    ib = coords_cartesian_size[0] * spot_params_focus_count - 1;
    for (ia = 0; ia <= ib; ia++) {
      r->data[ia] = 0.0;
    }

    for (count = 1; count <= spot_params_focus_count; count = (uint8_T)
         ((uint32_T)count + 1U)) {
      for (ia = 0; ia < 3; ia++) {
        b_center_coords[ia] = coords_data[(count + spot_params_focus_count * ia)
          - 1];
      }

      b_repmat(b_center_coords, (real_T)coords_cartesian_size[0], a_data, a_size);
      a_size[0] = coords_cartesian_size[0];
      a_size[1] = 3;
      ib = coords_cartesian_size[0] * coords_cartesian_size[1] - 1;
      for (ia = 0; ia <= ib; ia++) {
        a_data[ia] = coords_cartesian_data[ia] - a_data[ia];
      }

      eml_scalexp_alloc(a_data, a_size, y_data, y_size);
      ia = y_size[0] * 3;
      for (k = 0; k <= ia - 1; k++) {
        y_data[k] = rt_powd_snf(a_data[k], 2.0);
      }

      b_sum(y_data, y_size, x_data, x_size);
      ib = x_size[0] - 1;
      for (ia = 0; ia <= ib; ia++) {
        tmp_data[ia] = x_data[ia];
      }

      for (k = 0; k <= x_size[0] - 1; k++) {
        tmp_data[k] = sqrt(tmp_data[k]);
      }

      ib = x_size[0] - 1;
      for (ia = 0; ia <= ib; ia++) {
        r->data[ia + r->size[0] * (count - 1)] = tmp_data[ia];
      }
    }

    ia = y->size[0] * y->size[1];
    y->size[0] = r->size[0];
    y->size[1] = r->size[1];
    emxEnsureCapacity((emxArray__common *)y, ia, (int32_T)sizeof(creal_T));
    ib = r->size[0] * r->size[1] - 1;
    for (ia = 0; ia <= ib; ia++) {
      y->data[ia].re = r->data[ia] * -12.24;
      y->data[ia].im = r->data[ia] * -5446.2281821314446;
    }

    ia = y->size[0] * y->size[1];
    for (k = 0; k <= ia - 1; k++) {
      s_re = exp(y->data[k].re / 2.0);
      g_expl_temp = y->data[k].im;
      s_im = y->data[k].im;
      y->data[k].re = s_re * (s_re * cos(g_expl_temp));
      y->data[k].im = s_re * (s_re * sin(s_im));
    }

    mv[0] = 1;
    mv[1] = spot_params_focus_count;
    delta_area[0] = (int16_T)delta_area_size[0];
    delta_area[1] = 1;
    for (ia = 0; ia < 2; ia++) {
      outsize[ia] = (int16_T)(delta_area[ia] * mv[ia]);
    }

    ia = r1->size[0] * r1->size[1];
    r1->size[0] = outsize[0];
    r1->size[1] = outsize[1];
    emxEnsureCapacity((emxArray__common *)r1, ia, (int32_T)sizeof(real_T));
    if ((r1->size[0] == 0) || (r1->size[1] == 0)) {
    } else {
      ib = 0;
      for (vlen = 1; vlen <= mv[1]; vlen++) {
        ia = 0;
        for (k = 1; k <= delta_area_size[0]; k++) {
          r1->data[ib] = delta_area_data[ia];
          ia++;
          ib++;
        }
      }
    }

    ia = temp->size[0] * temp->size[1];
    temp->size[0] = y->size[0];
    temp->size[1] = y->size[1];
    emxEnsureCapacity((emxArray__common *)temp, ia, (int32_T)sizeof(creal_T));
    ib = y->size[0] * y->size[1] - 1;
    for (ia = 0; ia <= ib; ia++) {
      s_re = r1->data[ia] * y->data[ia].re;
      g_expl_temp = r1->data[ia] * y->data[ia].im;
      s_im = r->data[ia];
      if (g_expl_temp == 0.0) {
        temp->data[ia].re = s_re / s_im;
        temp->data[ia].im = 0.0;
      } else if (s_re == 0.0) {
        temp->data[ia].re = 0.0;
        temp->data[ia].im = g_expl_temp / s_im;
      } else {
        temp->data[ia].re = s_re / s_im;
        temp->data[ia].im = g_expl_temp / s_im;
      }
    }

    for (ia = 0; ia < 2; ia++) {
      sz[ia] = (int16_T)temp->size[ia];
    }

    sz[0] = 1;
    if ((temp->size[0] == 0) || (temp->size[1] == 0)) {
      ib = sz[1] - 1;
      for (ia = 0; ia <= ib; ia++) {
        b_y_data[ia].re = 0.0;
        b_y_data[ia].im = 0.0;
      }
    } else {
      vlen = temp->size[0];
      ia = temp->size[1];
      ix = -1;
      iy = -1;
      for (i = 1; i <= ia; i++) {
        ib = ix + 1;
        ix++;
        s_re = temp->data[ib].re;
        s_im = temp->data[ib].im;
        for (k = 2; k <= vlen; k++) {
          ix++;
          s_re += temp->data[ix].re;
          s_im += temp->data[ix].im;
        }

        iy++;
        b_y_data[iy].re = s_re;
        b_y_data[iy].im = s_im;
      }
    }

    vlen = H->size[0];
    ib = vlen - 1;
    for (ia = 0; ia <= ib; ia++) {
      H_data[ia].re = H->data[ia + H->size[0] * b_index].re * b_y_data[ia].re -
        H->data[ia + H->size[0] * b_index].im * b_y_data[ia].im;
      H_data[ia].im = H->data[ia + H->size[0] * b_index].re * b_y_data[ia].im +
        H->data[ia + H->size[0] * b_index].im * b_y_data[ia].re;
    }

    ib = vlen - 1;
    for (ia = 0; ia <= ib; ia++) {
      H->data[ia + H->size[0] * b_index] = H_data[ia];
    }
  }

  emxFree_creal_T(&y);
  emxFree_real_T(&r1);
  emxFree_creal_T(&temp);
  emxFree_real_T(&r);
  emxFree_real_T(&center_coords);
}

/* End of code generation (forward_matrix.c) */
