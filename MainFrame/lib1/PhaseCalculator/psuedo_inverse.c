/*
 * psuedo_inverse.c
 *
 * Code generation for function 'psuedo_inverse'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "psuedo_inverse.h"
#include "abs.h"
#include "sum.h"
#include "power.h"
#include "PhaseCalculator_emxutil.h"
#include "inv.h"
#include "round.h"
#include "angle.h"
#include "repmat.h"
#include "PhaseCalculator_rtwutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static void b_eml_xgemm(int32_T m, int32_T n, int32_T k, const emxArray_creal_T *
  A, int32_T lda, const emxArray_creal_T *B, int32_T ldb, emxArray_creal_T *C,
  int32_T ldc);

/* Function Definitions */
static void b_eml_xgemm(int32_T m, int32_T n, int32_T k, const emxArray_creal_T *
  A, int32_T lda, const emxArray_creal_T *B, int32_T ldb, emxArray_creal_T *C,
  int32_T ldc)
{
  int32_T c;
  int32_T cr;
  int32_T i17;
  int32_T ic;
  int32_T br;
  int32_T ar;
  int32_T ib;
  boolean_T b_B;
  real_T temp_re;
  real_T temp_im;
  int32_T ia;
  int32_T i18;
  real_T b_temp_re;
  real_T b_temp_im;
  if ((m == 0) || (n == 0)) {
  } else {
    c = ldc * (n - 1);
    cr = 0;
    while ((ldc > 0) && (cr <= c)) {
      i17 = cr + m;
      for (ic = cr; ic + 1 <= i17; ic++) {
        C->data[ic].re = 0.0;
        C->data[ic].im = 0.0;
      }

      cr += ldc;
    }

    br = 0;
    cr = 0;
    while ((ldc > 0) && (cr <= c)) {
      ar = -1;
      i17 = br + k;
      for (ib = br; ib + 1 <= i17; ib++) {
        b_B = ((B->data[ib].re != 0.0) || (B->data[ib].im != 0.0));
        if (b_B) {
          temp_re = B->data[ib].re - 0.0 * B->data[ib].im;
          temp_im = B->data[ib].im + 0.0 * B->data[ib].re;
          ia = ar;
          i18 = cr + m;
          for (ic = cr; ic + 1 <= i18; ic++) {
            ia++;
            b_temp_re = temp_re * A->data[ia].re - temp_im * A->data[ia].im;
            b_temp_im = temp_re * A->data[ia].im + temp_im * A->data[ia].re;
            C->data[ic].re += b_temp_re;
            C->data[ic].im += b_temp_im;
          }
        }

        ar += lda;
      }

      br += ldb;
      cr += ldc;
    }
  }
}

void psuedo_inverse(const emxArray_creal_T *H, uint8_T focus_count, real_T
                    pressure, uint8_T phase_data[144], int32_T phase_size[1])
{
  emxArray_creal_T *b;
  int32_T pressure_size[1];
  real_T pressure_data[255];
  int32_T i6;
  int32_T loop_ub;
  int32_T i7;
  emxArray_creal_T *y;
  uint8_T unnamed_idx_0;
  uint8_T unnamed_idx_1;
  int32_T m;
  int32_T b_loop_ub;
  emxArray_creal_T *a;
  int32_T k;
  int32_T i8;
  real_T efficiency;
  real_T mtmp;
  creal_T b_pressure_data[255];
  int32_T velocity_size[1];
  creal_T velocity_data[144];
  int32_T varargin_1_size[1];
  real_T varargin_1_data[144];
  int32_T b_varargin_1_size[1];
  real_T b_varargin_1_data[144];
  boolean_T exitg2;
  emxArray_real_T *matrix_weight;
  emxArray_creal_T *b_y;
  emxArray_creal_T *c_y;
  emxArray_creal_T *b_matrix_weight;
  emxArray_creal_T *c_matrix_weight;
  emxArray_creal_T *d_matrix_weight;
  emxArray_creal_T *e_matrix_weight;
  real_T matrix_weight_re;
  int32_T c_varargin_1_size[1];
  real_T c_varargin_1_data[144];
  boolean_T exitg1;
  emxInit_creal_T(&b, 2);
  c_repmat(1.0904850535426885E+6, focus_count, pressure_data, pressure_size);
  i6 = b->size[0] * b->size[1];
  b->size[0] = 144;
  b->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)b, i6, (int32_T)sizeof(creal_T));
  loop_ub = H->size[0] - 1;
  for (i6 = 0; i6 <= loop_ub; i6++) {
    for (i7 = 0; i7 < 144; i7++) {
      b->data[i7 + b->size[0] * i6].re = H->data[i6 + H->size[0] * i7].re;
      b->data[i7 + b->size[0] * i6].im = -H->data[i6 + H->size[0] * i7].im;
    }
  }

  emxInit_creal_T(&y, 2);
  unnamed_idx_0 = (uint8_T)H->size[0];
  unnamed_idx_1 = (uint8_T)b->size[1];
  i6 = y->size[0] * y->size[1];
  y->size[0] = unnamed_idx_0;
  y->size[1] = unnamed_idx_1;
  emxEnsureCapacity((emxArray__common *)y, i6, (int32_T)sizeof(creal_T));
  m = H->size[0];
  i6 = y->size[0] * y->size[1];
  y->size[0] = y->size[0];
  y->size[1] = y->size[1];
  emxEnsureCapacity((emxArray__common *)y, i6, (int32_T)sizeof(creal_T));
  loop_ub = y->size[1] - 1;
  for (i6 = 0; i6 <= loop_ub; i6++) {
    b_loop_ub = y->size[0] - 1;
    for (i7 = 0; i7 <= b_loop_ub; i7++) {
      y->data[i7 + y->size[0] * i6].re = 0.0;
      y->data[i7 + y->size[0] * i6].im = 0.0;
    }
  }

  emxInit_creal_T(&a, 2);
  b_eml_xgemm(m, b->size[1], 144, H, m, b, 144, y, m);
  i6 = a->size[0] * a->size[1];
  a->size[0] = 144;
  a->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)a, i6, (int32_T)sizeof(creal_T));
  loop_ub = H->size[0] - 1;
  for (i6 = 0; i6 <= loop_ub; i6++) {
    for (i7 = 0; i7 < 144; i7++) {
      a->data[i7 + a->size[0] * i6].re = H->data[i6 + H->size[0] * i7].re;
      a->data[i7 + a->size[0] * i6].im = -H->data[i6 + H->size[0] * i7].im;
    }
  }

  inv(y, b);
  if ((a->size[1] == 1) || (b->size[0] == 1)) {
    i6 = y->size[0] * y->size[1];
    y->size[0] = a->size[0];
    y->size[1] = b->size[1];
    emxEnsureCapacity((emxArray__common *)y, i6, (int32_T)sizeof(creal_T));
    loop_ub = a->size[0] - 1;
    for (i6 = 0; i6 <= loop_ub; i6++) {
      b_loop_ub = b->size[1] - 1;
      for (i7 = 0; i7 <= b_loop_ub; i7++) {
        y->data[i6 + y->size[0] * i7].re = 0.0;
        y->data[i6 + y->size[0] * i7].im = 0.0;
        k = a->size[1] - 1;
        for (i8 = 0; i8 <= k; i8++) {
          efficiency = a->data[i6 + a->size[0] * i8].re * b->data[i8 + b->size[0]
            * i7].re - a->data[i6 + a->size[0] * i8].im * b->data[i8 + b->size[0]
            * i7].im;
          mtmp = a->data[i6 + a->size[0] * i8].re * b->data[i8 + b->size[0] * i7]
            .im + a->data[i6 + a->size[0] * i8].im * b->data[i8 + b->size[0] *
            i7].re;
          y->data[i6 + y->size[0] * i7].re += efficiency;
          y->data[i6 + y->size[0] * i7].im += mtmp;
        }
      }
    }
  } else {
    k = a->size[1];
    unnamed_idx_1 = (uint8_T)b->size[1];
    i6 = y->size[0] * y->size[1];
    y->size[0] = 144;
    y->size[1] = unnamed_idx_1;
    y->size[0] = y->size[0];
    y->size[1] = y->size[1];
    emxEnsureCapacity((emxArray__common *)y, i6, (int32_T)sizeof(creal_T));
    loop_ub = y->size[1] - 1;
    for (i6 = 0; i6 <= loop_ub; i6++) {
      b_loop_ub = y->size[0] - 1;
      for (i7 = 0; i7 <= b_loop_ub; i7++) {
        y->data[i7 + y->size[0] * i6].re = 0.0;
        y->data[i7 + y->size[0] * i6].im = 0.0;
      }
    }

    b_eml_xgemm(144, b->size[1], k, a, 144, b, k, y, 144);
  }

  emxFree_creal_T(&a);
  loop_ub = pressure_size[0] - 1;
  for (i6 = 0; i6 <= loop_ub; i6++) {
    b_pressure_data[i6].re = pressure_data[i6];
    b_pressure_data[i6].im = 0.0;
  }

  velocity_size[0] = y->size[0];
  loop_ub = y->size[0] - 1;
  for (i6 = 0; i6 <= loop_ub; i6++) {
    velocity_data[i6].re = 0.0;
    velocity_data[i6].im = 0.0;
    b_loop_ub = pressure_size[0] - 1;
    for (i7 = 0; i7 <= b_loop_ub; i7++) {
      efficiency = y->data[i6 + y->size[0] * i7].re * b_pressure_data[i7].re -
        y->data[i6 + y->size[0] * i7].im * 0.0;
      mtmp = y->data[i6 + y->size[0] * i7].re * 0.0 + y->data[i6 + y->size[0] *
        i7].im * b_pressure_data[i7].re;
      velocity_data[i6].re += efficiency;
      velocity_data[i6].im += mtmp;
    }
  }

  c_abs(velocity_data, velocity_size, varargin_1_data, varargin_1_size);
  b_varargin_1_size[0] = 144;
  memcpy(&b_varargin_1_data[0], &varargin_1_data[0], 144U * sizeof(real_T));
  power(b_varargin_1_data, b_varargin_1_size, varargin_1_data, varargin_1_size);
  efficiency = sum(varargin_1_data, varargin_1_size);
  c_abs(velocity_data, velocity_size, varargin_1_data, varargin_1_size);
  k = 1;
  mtmp = varargin_1_data[0];
  if (rtIsNaN(varargin_1_data[0])) {
    m = 2;
    exitg2 = FALSE;
    while ((exitg2 == 0U) && (m <= 144)) {
      k = m;
      if (!rtIsNaN(varargin_1_data[m - 1])) {
        mtmp = varargin_1_data[m - 1];
        exitg2 = TRUE;
      } else {
        m++;
      }
    }
  }

  if (k < 144) {
    while (k + 1 <= 144) {
      if (varargin_1_data[k] > mtmp) {
        mtmp = varargin_1_data[k];
      }

      k++;
    }
  }

  emxInit_real_T(&matrix_weight, 2);
  efficiency = efficiency / 144.0 / rt_powd_snf(mtmp, 2.0);

  /*  code generation requires matrix_weight to the predetermined size */
  i6 = matrix_weight->size[0] * matrix_weight->size[1];
  matrix_weight->size[0] = 144;
  matrix_weight->size[1] = 144;
  emxEnsureCapacity((emxArray__common *)matrix_weight, i6, (int32_T)sizeof
                    (real_T));
  for (i6 = 0; i6 < 20736; i6++) {
    matrix_weight->data[i6] = 0.0;
  }

  emxInit_creal_T(&b_y, 2);
  emxInit_creal_T(&c_y, 2);
  emxInit_creal_T(&b_matrix_weight, 2);
  emxInit_creal_T(&c_matrix_weight, 2);
  emxInit_creal_T(&d_matrix_weight, 2);
  emxInit_creal_T(&e_matrix_weight, 2);
  while (efficiency < 0.95) {
    for (k = 0; k < 144; k++) {
      matrix_weight->data[k + matrix_weight->size[0] * k] = 1.0 / b_abs
        (velocity_data[k]);
    }

    /*  comment because code generation could not support sparse */
    /*      matrix_weight = sparse(1:element_count, 1:element_count, 1./abs(velocity)); */
    i6 = b_matrix_weight->size[0] * b_matrix_weight->size[1];
    b_matrix_weight->size[0] = 144;
    b_matrix_weight->size[1] = 144;
    emxEnsureCapacity((emxArray__common *)b_matrix_weight, i6, (int32_T)sizeof
                      (creal_T));
    for (i6 = 0; i6 < 144; i6++) {
      for (i7 = 0; i7 < 144; i7++) {
        b_matrix_weight->data[i7 + b_matrix_weight->size[0] * i6].re =
          matrix_weight->data[i7 + matrix_weight->size[0] * i6];
        b_matrix_weight->data[i7 + b_matrix_weight->size[0] * i6].im = 0.0;
      }
    }

    i6 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = 144;
    b_y->size[1] = H->size[0];
    emxEnsureCapacity((emxArray__common *)b_y, i6, (int32_T)sizeof(creal_T));
    for (i6 = 0; i6 < 144; i6++) {
      loop_ub = H->size[0] - 1;
      for (i7 = 0; i7 <= loop_ub; i7++) {
        b_y->data[i6 + b_y->size[0] * i7].re = 0.0;
        b_y->data[i6 + b_y->size[0] * i7].im = 0.0;
        for (i8 = 0; i8 < 144; i8++) {
          efficiency = H->data[i7 + H->size[0] * i8].re;
          mtmp = -H->data[i7 + H->size[0] * i8].im;
          matrix_weight_re = b_matrix_weight->data[i6 + b_matrix_weight->size[0]
            * i8].re * efficiency - b_matrix_weight->data[i6 +
            b_matrix_weight->size[0] * i8].im * mtmp;
          efficiency = b_matrix_weight->data[i6 + b_matrix_weight->size[0] * i8]
            .re * mtmp + b_matrix_weight->data[i6 + b_matrix_weight->size[0] *
            i8].im * efficiency;
          b_y->data[i6 + b_y->size[0] * i7].re += matrix_weight_re;
          b_y->data[i6 + b_y->size[0] * i7].im += efficiency;
        }
      }
    }

    i6 = c_matrix_weight->size[0] * c_matrix_weight->size[1];
    c_matrix_weight->size[0] = 144;
    c_matrix_weight->size[1] = 144;
    emxEnsureCapacity((emxArray__common *)c_matrix_weight, i6, (int32_T)sizeof
                      (creal_T));
    for (i6 = 0; i6 < 144; i6++) {
      for (i7 = 0; i7 < 144; i7++) {
        c_matrix_weight->data[i7 + c_matrix_weight->size[0] * i6].re =
          matrix_weight->data[i7 + matrix_weight->size[0] * i6];
        c_matrix_weight->data[i7 + c_matrix_weight->size[0] * i6].im = 0.0;
      }
    }

    i6 = c_y->size[0] * c_y->size[1];
    c_y->size[0] = 144;
    c_y->size[1] = H->size[0];
    emxEnsureCapacity((emxArray__common *)c_y, i6, (int32_T)sizeof(creal_T));
    for (i6 = 0; i6 < 144; i6++) {
      loop_ub = H->size[0] - 1;
      for (i7 = 0; i7 <= loop_ub; i7++) {
        c_y->data[i6 + c_y->size[0] * i7].re = 0.0;
        c_y->data[i6 + c_y->size[0] * i7].im = 0.0;
        for (i8 = 0; i8 < 144; i8++) {
          efficiency = H->data[i7 + H->size[0] * i8].re;
          mtmp = -H->data[i7 + H->size[0] * i8].im;
          matrix_weight_re = c_matrix_weight->data[i6 + c_matrix_weight->size[0]
            * i8].re * efficiency - c_matrix_weight->data[i6 +
            c_matrix_weight->size[0] * i8].im * mtmp;
          efficiency = c_matrix_weight->data[i6 + c_matrix_weight->size[0] * i8]
            .re * mtmp + c_matrix_weight->data[i6 + c_matrix_weight->size[0] *
            i8].im * efficiency;
          c_y->data[i6 + c_y->size[0] * i7].re += matrix_weight_re;
          c_y->data[i6 + c_y->size[0] * i7].im += efficiency;
        }
      }
    }

    unnamed_idx_0 = (uint8_T)H->size[0];
    unnamed_idx_1 = (uint8_T)c_y->size[1];
    i6 = y->size[0] * y->size[1];
    y->size[0] = unnamed_idx_0;
    y->size[1] = unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)y, i6, (int32_T)sizeof(creal_T));
    m = H->size[0];
    i6 = y->size[0] * y->size[1];
    y->size[0] = y->size[0];
    y->size[1] = y->size[1];
    emxEnsureCapacity((emxArray__common *)y, i6, (int32_T)sizeof(creal_T));
    loop_ub = y->size[1] - 1;
    for (i6 = 0; i6 <= loop_ub; i6++) {
      b_loop_ub = y->size[0] - 1;
      for (i7 = 0; i7 <= b_loop_ub; i7++) {
        y->data[i7 + y->size[0] * i6].re = 0.0;
        y->data[i7 + y->size[0] * i6].im = 0.0;
      }
    }

    i6 = d_matrix_weight->size[0] * d_matrix_weight->size[1];
    d_matrix_weight->size[0] = 144;
    d_matrix_weight->size[1] = 144;
    emxEnsureCapacity((emxArray__common *)d_matrix_weight, i6, (int32_T)sizeof
                      (creal_T));
    for (i6 = 0; i6 < 144; i6++) {
      for (i7 = 0; i7 < 144; i7++) {
        d_matrix_weight->data[i7 + d_matrix_weight->size[0] * i6].re =
          matrix_weight->data[i7 + matrix_weight->size[0] * i6];
        d_matrix_weight->data[i7 + d_matrix_weight->size[0] * i6].im = 0.0;
      }
    }

    i6 = e_matrix_weight->size[0] * e_matrix_weight->size[1];
    e_matrix_weight->size[0] = 144;
    e_matrix_weight->size[1] = H->size[0];
    emxEnsureCapacity((emxArray__common *)e_matrix_weight, i6, (int32_T)sizeof
                      (creal_T));
    for (i6 = 0; i6 < 144; i6++) {
      loop_ub = H->size[0] - 1;
      for (i7 = 0; i7 <= loop_ub; i7++) {
        e_matrix_weight->data[i6 + e_matrix_weight->size[0] * i7].re = 0.0;
        e_matrix_weight->data[i6 + e_matrix_weight->size[0] * i7].im = 0.0;
        for (i8 = 0; i8 < 144; i8++) {
          efficiency = H->data[i7 + H->size[0] * i8].re;
          mtmp = -H->data[i7 + H->size[0] * i8].im;
          matrix_weight_re = d_matrix_weight->data[i6 + d_matrix_weight->size[0]
            * i8].re * efficiency - d_matrix_weight->data[i6 +
            d_matrix_weight->size[0] * i8].im * mtmp;
          efficiency = d_matrix_weight->data[i6 + d_matrix_weight->size[0] * i8]
            .re * mtmp + d_matrix_weight->data[i6 + d_matrix_weight->size[0] *
            i8].im * efficiency;
          e_matrix_weight->data[i6 + e_matrix_weight->size[0] * i7].re +=
            matrix_weight_re;
          e_matrix_weight->data[i6 + e_matrix_weight->size[0] * i7].im +=
            efficiency;
        }
      }
    }

    b_eml_xgemm(m, e_matrix_weight->size[1], 144, H, m, c_y, 144, y, m);
    inv(y, b);
    if ((b_y->size[1] == 1) || (b->size[0] == 1)) {
      i6 = y->size[0] * y->size[1];
      y->size[0] = 144;
      y->size[1] = b->size[1];
      emxEnsureCapacity((emxArray__common *)y, i6, (int32_T)sizeof(creal_T));
      for (i6 = 0; i6 < 144; i6++) {
        loop_ub = b->size[1] - 1;
        for (i7 = 0; i7 <= loop_ub; i7++) {
          y->data[i6 + y->size[0] * i7].re = 0.0;
          y->data[i6 + y->size[0] * i7].im = 0.0;
          b_loop_ub = b_y->size[1] - 1;
          for (i8 = 0; i8 <= b_loop_ub; i8++) {
            efficiency = b_y->data[i6 + b_y->size[0] * i8].re * b->data[i8 +
              b->size[0] * i7].re - b_y->data[i6 + b_y->size[0] * i8].im *
              b->data[i8 + b->size[0] * i7].im;
            mtmp = b_y->data[i6 + b_y->size[0] * i8].re * b->data[i8 + b->size[0]
              * i7].im + b_y->data[i6 + b_y->size[0] * i8].im * b->data[i8 +
              b->size[0] * i7].re;
            y->data[i6 + y->size[0] * i7].re += efficiency;
            y->data[i6 + y->size[0] * i7].im += mtmp;
          }
        }
      }
    } else {
      k = b_y->size[1];
      unnamed_idx_1 = (uint8_T)b->size[1];
      i6 = y->size[0] * y->size[1];
      y->size[0] = 144;
      y->size[1] = unnamed_idx_1;
      y->size[0] = y->size[0];
      y->size[1] = y->size[1];
      emxEnsureCapacity((emxArray__common *)y, i6, (int32_T)sizeof(creal_T));
      loop_ub = y->size[1] - 1;
      for (i6 = 0; i6 <= loop_ub; i6++) {
        b_loop_ub = y->size[0] - 1;
        for (i7 = 0; i7 <= b_loop_ub; i7++) {
          y->data[i7 + y->size[0] * i6].re = 0.0;
          y->data[i7 + y->size[0] * i6].im = 0.0;
        }
      }

      b_eml_xgemm(144, b->size[1], k, b_y, 144, b, k, y, 144);
    }

    loop_ub = pressure_size[0] - 1;
    for (i6 = 0; i6 <= loop_ub; i6++) {
      b_pressure_data[i6].re = pressure_data[i6];
      b_pressure_data[i6].im = 0.0;
    }

    velocity_size[0] = y->size[0];
    loop_ub = y->size[0] - 1;
    for (i6 = 0; i6 <= loop_ub; i6++) {
      velocity_data[i6].re = 0.0;
      velocity_data[i6].im = 0.0;
      b_loop_ub = pressure_size[0] - 1;
      for (i7 = 0; i7 <= b_loop_ub; i7++) {
        efficiency = y->data[i6 + y->size[0] * i7].re * b_pressure_data[i7].re -
          y->data[i6 + y->size[0] * i7].im * 0.0;
        mtmp = y->data[i6 + y->size[0] * i7].re * 0.0 + y->data[i6 + y->size[0] *
          i7].im * b_pressure_data[i7].re;
        velocity_data[i6].re += efficiency;
        velocity_data[i6].im += mtmp;
      }
    }

    c_abs(velocity_data, velocity_size, varargin_1_data, varargin_1_size);
    c_varargin_1_size[0] = 144;
    memcpy(&c_varargin_1_data[0], &varargin_1_data[0], 144U * sizeof(real_T));
    power(c_varargin_1_data, c_varargin_1_size, varargin_1_data, varargin_1_size);
    efficiency = sum(varargin_1_data, varargin_1_size);
    c_abs(velocity_data, velocity_size, varargin_1_data, varargin_1_size);
    k = 1;
    mtmp = varargin_1_data[0];
    if (rtIsNaN(varargin_1_data[0])) {
      m = 2;
      exitg1 = FALSE;
      while ((exitg1 == 0U) && (m <= 144)) {
        k = m;
        if (!rtIsNaN(varargin_1_data[m - 1])) {
          mtmp = varargin_1_data[m - 1];
          exitg1 = TRUE;
        } else {
          m++;
        }
      }
    }

    if (k < 144) {
      while (k + 1 <= 144) {
        if (varargin_1_data[k] > mtmp) {
          mtmp = varargin_1_data[k];
        }

        k++;
      }
    }

    efficiency = efficiency / 144.0 / rt_powd_snf(mtmp, 2.0);
  }

  emxFree_creal_T(&e_matrix_weight);
  emxFree_creal_T(&d_matrix_weight);
  emxFree_creal_T(&c_matrix_weight);
  emxFree_creal_T(&b_matrix_weight);
  emxFree_creal_T(&c_y);
  emxFree_creal_T(&b_y);
  emxFree_creal_T(&b);
  emxFree_creal_T(&y);
  emxFree_real_T(&matrix_weight);

  /*  only for single-focus pattern */
  /*  phase = angle(H') + angle(pressure); */
  /*  amp = pressure / (H * (H./abs(H))'); */
  /*  velocity = amp * exp(1i * phase); */
  angle(velocity_data, velocity_size, varargin_1_data, varargin_1_size);
  varargin_1_size[0] = 144;
  for (i6 = 0; i6 < 144; i6++) {
    varargin_1_data[i6] = (varargin_1_data[i6] * 180.0 / 3.1415926535897931 +
      180.0) / 1.40625;
  }

  b_round(varargin_1_data, varargin_1_size);
  phase_size[0] = 144;
  for (i6 = 0; i6 < 144; i6++) {
    efficiency = rt_roundd_snf(varargin_1_data[i6]);
    if (efficiency < 256.0) {
      if (efficiency >= 0.0) {
        unnamed_idx_0 = (uint8_T)efficiency;
      } else {
        unnamed_idx_0 = 0;
      }
    } else if (efficiency >= 256.0) {
      unnamed_idx_0 = MAX_uint8_T;
    } else {
      unnamed_idx_0 = 0;
    }

    phase_data[i6] = unnamed_idx_0;
  }
}

/* End of code generation (psuedo_inverse.c) */
