/*
 * set_array_params.c
 *
 * Code generation for function 'set_array_params'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "set_array_params.h"
#include "PhaseCalculator_emxutil.h"
#include "colon.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */

/* Function Definitions */
void set_array_params(real_T *array_params_roc, real_T *array_params_radius,
                      real_T *array_params_count_in_radius, real_T
                      *array_params_count_in_angle, real_T
                      *array_params_ring_count, real_T
                      c_array_params_element_in_ring_[6], real_T
                      *array_params_element_count, real_T
                      array_params_angle_with_propa[6], real_T
                      c_array_params_angle_vertical_p[6], emxArray_real_T
                      *array_params_angles_with_propa, emxArray_real_T
                      *c_array_params_angles_vertical_)
{
  emxArray_real_T *angles_with_propa;
  emxArray_real_T *angles_vertical_propa;
  int32_T nm1;
  emxArray_real_T *b_angles_with_propa;
  emxArray_real_T *b_angles_vertical_propa;
  int32_T i;
  int32_T nm1d2;
  static const real_T dv0[6] = { 0.41154863762026284, 0.46547931150688771,
    0.51312680008633282, 0.5536184387326013, 0.59847340050885556,
    0.64158303303311548 };

  boolean_T n_too_large;
  real_T bnew;
  real_T anew;
  int32_T n;
  static const real_T dv1[6] = { 0.0, 0.1265363707695889, 0.0,
    0.1265363707695889, 0.0, 0.1265363707695889 };

  real_T y_data[24];
  int32_T k;
  real_T kd;
  static const real_T angle_with_propa[6] = { 0.41154863762026284,
    0.46547931150688771, 0.51312680008633282, 0.5536184387326013,
    0.59847340050885556, 0.64158303303311548 };

  static const real_T angle_vertical_propa[6] = { 0.0, 0.1265363707695889, 0.0,
    0.1265363707695889, 0.0, 0.1265363707695889 };

  emxInit_real_T(&angles_with_propa, 2);
  emxInit_real_T(&angles_vertical_propa, 2);

  /*    2016/03/02 */
  /*    JI Xiang */
  /*    Radius of curvature: m */
  /*    Radius of element: m */
  /*    Divide an element in the directions of raidus and angle. */
  /*    Ring number in the array */
  /*    Element number in each ring of the array */
  /*  The element number in the array */
  /*    The angle between the ring and the propagation axis: rad */
  /*    The angle of the first element in the ring in the plane perpendicular */
  /*    with propagation: rad */
  /*     */
  nm1 = angles_with_propa->size[0] * angles_with_propa->size[1];
  angles_with_propa->size[0] = 1;
  angles_with_propa->size[1] = 0;
  emxEnsureCapacity((emxArray__common *)angles_with_propa, nm1, (int32_T)sizeof
                    (real_T));
  nm1 = angles_vertical_propa->size[0] * angles_vertical_propa->size[1];
  angles_vertical_propa->size[0] = 1;
  angles_vertical_propa->size[1] = 0;
  emxEnsureCapacity((emxArray__common *)angles_vertical_propa, nm1, (int32_T)
                    sizeof(real_T));
  emxInit_real_T(&b_angles_with_propa, 2);
  emxInit_real_T(&b_angles_vertical_propa, 2);
  for (i = 0; i < 6; i++) {
    nm1 = b_angles_with_propa->size[0] * b_angles_with_propa->size[1];
    b_angles_with_propa->size[0] = 1;
    b_angles_with_propa->size[1] = angles_with_propa->size[1] + 24;
    emxEnsureCapacity((emxArray__common *)b_angles_with_propa, nm1, (int32_T)
                      sizeof(real_T));
    nm1d2 = angles_with_propa->size[1] - 1;
    for (nm1 = 0; nm1 <= nm1d2; nm1++) {
      b_angles_with_propa->data[b_angles_with_propa->size[0] * nm1] =
        angles_with_propa->data[angles_with_propa->size[0] * nm1];
    }

    for (nm1 = 0; nm1 < 24; nm1++) {
      b_angles_with_propa->data[b_angles_with_propa->size[0] * (nm1 +
        angles_with_propa->size[1])] = dv0[i];
    }

    nm1 = angles_with_propa->size[0] * angles_with_propa->size[1];
    angles_with_propa->size[0] = 1;
    angles_with_propa->size[1] = b_angles_with_propa->size[1];
    emxEnsureCapacity((emxArray__common *)angles_with_propa, nm1, (int32_T)
                      sizeof(real_T));
    nm1d2 = b_angles_with_propa->size[1] - 1;
    for (nm1 = 0; nm1 <= nm1d2; nm1++) {
      angles_with_propa->data[angles_with_propa->size[0] * nm1] =
        b_angles_with_propa->data[b_angles_with_propa->size[0] * nm1];
    }

    float_colon_length(dv1[i], 0.26179938779914941, dv1[i] + 6.0213859193804362,
                       &n, &anew, &bnew, &n_too_large);
    if (n > 0) {
      y_data[0] = anew;
      if (n > 1) {
        y_data[n - 1] = bnew;
        nm1 = n - 1;
        nm1d2 = nm1 / 2;
        for (k = 1; k <= nm1d2 - 1; k++) {
          kd = (real_T)k * 0.26179938779914941;
          y_data[k] = anew + kd;
          y_data[(n - k) - 1] = bnew - kd;
        }

        if (nm1d2 << 1 == nm1) {
          y_data[nm1d2] = (anew + bnew) / 2.0;
        } else {
          kd = (real_T)nm1d2 * 0.26179938779914941;
          y_data[nm1d2] = anew + kd;
          y_data[nm1d2 + 1] = bnew - kd;
        }
      }
    }

    nm1 = b_angles_vertical_propa->size[0] * b_angles_vertical_propa->size[1];
    b_angles_vertical_propa->size[0] = 1;
    b_angles_vertical_propa->size[1] = angles_vertical_propa->size[1] + n;
    emxEnsureCapacity((emxArray__common *)b_angles_vertical_propa, nm1, (int32_T)
                      sizeof(real_T));
    nm1d2 = angles_vertical_propa->size[1] - 1;
    for (nm1 = 0; nm1 <= nm1d2; nm1++) {
      b_angles_vertical_propa->data[b_angles_vertical_propa->size[0] * nm1] =
        angles_vertical_propa->data[angles_vertical_propa->size[0] * nm1];
    }

    nm1d2 = n - 1;
    for (nm1 = 0; nm1 <= nm1d2; nm1++) {
      b_angles_vertical_propa->data[b_angles_vertical_propa->size[0] * (nm1 +
        angles_vertical_propa->size[1])] = y_data[nm1];
    }

    nm1 = angles_vertical_propa->size[0] * angles_vertical_propa->size[1];
    angles_vertical_propa->size[0] = 1;
    angles_vertical_propa->size[1] = b_angles_vertical_propa->size[1];
    emxEnsureCapacity((emxArray__common *)angles_vertical_propa, nm1, (int32_T)
                      sizeof(real_T));
    nm1d2 = b_angles_vertical_propa->size[1] - 1;
    for (nm1 = 0; nm1 <= nm1d2; nm1++) {
      angles_vertical_propa->data[angles_vertical_propa->size[0] * nm1] =
        b_angles_vertical_propa->data[b_angles_vertical_propa->size[0] * nm1];
    }
  }

  emxFree_real_T(&b_angles_vertical_propa);
  emxFree_real_T(&b_angles_with_propa);
  *array_params_roc = 0.14;
  *array_params_radius = 0.005;
  *array_params_count_in_radius = 20.0;
  *array_params_count_in_angle = 20.0;
  *array_params_ring_count = 6.0;
  for (nm1 = 0; nm1 < 6; nm1++) {
    c_array_params_element_in_ring_[nm1] = 24.0;
  }

  *array_params_element_count = 144.0;
  for (nm1 = 0; nm1 < 6; nm1++) {
    array_params_angle_with_propa[nm1] = angle_with_propa[nm1];
  }

  for (nm1 = 0; nm1 < 6; nm1++) {
    c_array_params_angle_vertical_p[nm1] = angle_vertical_propa[nm1];
  }

  nm1 = array_params_angles_with_propa->size[0] *
    array_params_angles_with_propa->size[1];
  array_params_angles_with_propa->size[0] = 1;
  array_params_angles_with_propa->size[1] = angles_with_propa->size[1];
  emxEnsureCapacity((emxArray__common *)array_params_angles_with_propa, nm1,
                    (int32_T)sizeof(real_T));
  nm1d2 = angles_with_propa->size[0] * angles_with_propa->size[1] - 1;
  for (nm1 = 0; nm1 <= nm1d2; nm1++) {
    array_params_angles_with_propa->data[nm1] = angles_with_propa->data[nm1];
  }

  emxFree_real_T(&angles_with_propa);
  nm1 = c_array_params_angles_vertical_->size[0] *
    c_array_params_angles_vertical_->size[1];
  c_array_params_angles_vertical_->size[0] = 1;
  c_array_params_angles_vertical_->size[1] = angles_vertical_propa->size[1];
  emxEnsureCapacity((emxArray__common *)c_array_params_angles_vertical_, nm1,
                    (int32_T)sizeof(real_T));
  nm1d2 = angles_vertical_propa->size[0] * angles_vertical_propa->size[1] - 1;
  for (nm1 = 0; nm1 <= nm1d2; nm1++) {
    c_array_params_angles_vertical_->data[nm1] = angles_vertical_propa->data[nm1];
  }

  emxFree_real_T(&angles_vertical_propa);
}

/* End of code generation (set_array_params.c) */
