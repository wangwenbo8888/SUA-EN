/*
 * inv.c
 *
 * Code generation for function 'inv'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "inv.h"
#include "PhaseCalculator_emxutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static creal_T eml_div(const creal_T x, const creal_T y);
static void eml_ipiv2perm(const int32_T ipiv_data[255], const int32_T ipiv_size
  [2], int32_T m, int32_T perm_data[255], int32_T perm_size[2]);
static void eml_xswap(int32_T n, emxArray_creal_T *x, int32_T ix0, int32_T incx,
                      int32_T iy0, int32_T incy);
static void eml_xtrsm(int32_T m, int32_T n, const emxArray_creal_T *A, int32_T
                      lda, emxArray_creal_T *B, int32_T ldb);
static void invNxN(const emxArray_creal_T *x, emxArray_creal_T *y);

/* Function Definitions */
static creal_T eml_div(const creal_T x, const creal_T y)
{
  creal_T z;
  real_T brm;
  real_T bim;
  real_T d;
  if (y.im == 0.0) {
    if (x.im == 0.0) {
      z.re = x.re / y.re;
      z.im = 0.0;
    } else if (x.re == 0.0) {
      z.re = 0.0;
      z.im = x.im / y.re;
    } else {
      z.re = x.re / y.re;
      z.im = x.im / y.re;
    }
  } else if (y.re == 0.0) {
    if (x.re == 0.0) {
      z.re = x.im / y.im;
      z.im = 0.0;
    } else if (x.im == 0.0) {
      z.re = 0.0;
      z.im = -(x.re / y.im);
    } else {
      z.re = x.im / y.im;
      z.im = -(x.re / y.im);
    }
  } else {
    brm = fabs(y.re);
    bim = fabs(y.im);
    if (brm > bim) {
      bim = y.im / y.re;
      d = y.re + bim * y.im;
      z.re = (x.re + bim * x.im) / d;
      z.im = (x.im - bim * x.re) / d;
    } else if (bim == brm) {
      bim = y.re > 0.0 ? 0.5 : -0.5;
      d = y.im > 0.0 ? 0.5 : -0.5;
      z.re = (x.re * bim + x.im * d) / brm;
      z.im = (x.im * bim - x.re * d) / brm;
    } else {
      bim = y.re / y.im;
      d = y.im + bim * y.re;
      z.re = (bim * x.re + x.im) / d;
      z.im = (bim * x.im - x.re) / d;
    }
  }

  return z;
}

static void eml_ipiv2perm(const int32_T ipiv_data[255], const int32_T ipiv_size
  [2], int32_T m, int32_T perm_data[255], int32_T perm_size[2])
{
  int32_T n;
  int32_T yk;
  int32_T k;
  if (m < 1) {
    n = 0;
  } else {
    n = m;
  }

  perm_size[0] = 1;
  perm_size[1] = n;
  if (n > 0) {
    perm_data[0] = 1;
    yk = 1;
    for (k = 2; k <= n; k++) {
      yk++;
      perm_data[k - 1] = yk;
    }
  }

  for (k = 0; k <= ipiv_size[1] - 1; k++) {
    if (ipiv_data[k] > 1 + k) {
      n = perm_data[ipiv_data[k] - 1];
      perm_data[ipiv_data[k] - 1] = perm_data[k];
      perm_data[k] = n;
    }
  }
}

static void eml_xswap(int32_T n, emxArray_creal_T *x, int32_T ix0, int32_T incx,
                      int32_T iy0, int32_T incy)
{
  int32_T ix;
  int32_T iy;
  int32_T k;
  real_T temp_re;
  real_T temp_im;
  ix = ix0 - 1;
  iy = iy0 - 1;
  for (k = 1; k <= n; k++) {
    temp_re = x->data[ix].re;
    temp_im = x->data[ix].im;
    x->data[ix] = x->data[iy];
    x->data[iy].re = temp_re;
    x->data[iy].im = temp_im;
    ix += incx;
    iy += incy;
  }
}

static void eml_xtrsm(int32_T m, int32_T n, const emxArray_creal_T *A, int32_T
                      lda, emxArray_creal_T *B, int32_T ldb)
{
  int32_T j;
  int32_T c;
  int32_T k;
  int32_T b_c;
  boolean_T b_B;
  creal_T c_B;
  creal_T b_A;
  int32_T i;
  real_T B_re;
  real_T B_im;
  if ((n == 0) || ((B->size[0] == 0) || (B->size[1] == 0))) {
  } else {
    for (j = 1; j <= n; j++) {
      c = ldb * (j - 1);
      for (k = m - 1; k + 1 > 0; k--) {
        b_c = lda * k;
        b_B = ((B->data[k + c].re != 0.0) || (B->data[k + c].im != 0.0));
        if (b_B) {
          c_B = B->data[k + c];
          b_A = A->data[k + b_c];
          B->data[k + c] = eml_div(c_B, b_A);
          for (i = 0; i + 1 <= k; i++) {
            B_re = B->data[k + c].re * A->data[i + b_c].re - B->data[k + c].im *
              A->data[i + b_c].im;
            B_im = B->data[k + c].re * A->data[i + b_c].im + B->data[k + c].im *
              A->data[i + b_c].re;
            B->data[i + c].re -= B_re;
            B->data[i + c].im -= B_im;
          }
        }
      }
    }
  }
}

static void invNxN(const emxArray_creal_T *x, emxArray_creal_T *y)
{
  int32_T n;
  int32_T i10;
  int32_T k;
  int32_T b_n;
  int32_T yk;
  emxArray_creal_T *A;
  int32_T p_data[255];
  int32_T p_size[2];
  int32_T ldap1;
  int32_T u0;
  int32_T j;
  int32_T mmj;
  int32_T jp1j;
  int32_T c;
  int32_T ix;
  real_T smax;
  real_T s;
  boolean_T b_A;
  creal_T c_A;
  creal_T d_A;
  int32_T jA;
  real_T A_re;
  real_T A_im;
  int32_T b_p_data[255];
  int32_T b_p_size[2];
  n = x->size[0];
  for (i10 = 0; i10 < 2; i10++) {
    k = y->size[0] * y->size[1];
    y->size[i10] = x->size[i10];
    emxEnsureCapacity((emxArray__common *)y, k, (int32_T)sizeof(creal_T));
  }

  i10 = y->size[0] * y->size[1];
  y->size[0] = y->size[0];
  y->size[1] = y->size[1];
  emxEnsureCapacity((emxArray__common *)y, i10, (int32_T)sizeof(creal_T));
  b_n = y->size[1] - 1;
  for (i10 = 0; i10 <= b_n; i10++) {
    yk = y->size[0] - 1;
    for (k = 0; k <= yk; k++) {
      y->data[k + y->size[0] * i10].re = 0.0;
      y->data[k + y->size[0] * i10].im = 0.0;
    }
  }

  emxInit_creal_T(&A, 2);
  i10 = A->size[0] * A->size[1];
  A->size[0] = x->size[0];
  A->size[1] = x->size[1];
  emxEnsureCapacity((emxArray__common *)A, i10, (int32_T)sizeof(creal_T));
  b_n = x->size[0] * x->size[1] - 1;
  for (i10 = 0; i10 <= b_n; i10++) {
    A->data[i10] = x->data[i10];
  }

  if (n < 1) {
    b_n = 0;
  } else {
    b_n = n;
  }

  p_size[0] = 1;
  p_size[1] = b_n;
  if (b_n > 0) {
    p_data[0] = 1;
    yk = 1;
    for (k = 2; k <= b_n; k++) {
      yk++;
      p_data[k - 1] = yk;
    }
  }

  if (n < 1) {
  } else {
    ldap1 = n + 1;
    u0 = n - 1;
    if (u0 <= n) {
    } else {
      u0 = n;
    }

    for (j = 1; j <= u0; j++) {
      mmj = n - j;
      yk = (j - 1) * ldap1;
      jp1j = yk + 2;
      c = mmj + 1;
      if (c < 1) {
        b_n = 0;
      } else {
        b_n = 1;
        if (c > 1) {
          ix = yk;
          smax = fabs(A->data[yk].re) + fabs(A->data[yk].im);
          for (k = 2; k <= c; k++) {
            ix++;
            s = fabs(A->data[ix].re) + fabs(A->data[ix].im);
            if (s > smax) {
              b_n = k;
              smax = s;
            }
          }
        }
      }

      b_A = ((A->data[(yk + b_n) - 1].re != 0.0) || (A->data[(yk + b_n) - 1].im
              != 0.0));
      if (b_A) {
        if (b_n - 1 != 0) {
          p_data[j - 1] = (j + b_n) - 1;
          eml_xswap(n, A, j, n, (j + b_n) - 1, n);
        }

        i10 = (jp1j + mmj) - 1;
        for (b_n = jp1j; b_n <= i10; b_n++) {
          c_A = A->data[b_n - 1];
          d_A = A->data[yk];
          A->data[b_n - 1] = eml_div(c_A, d_A);
        }
      }

      c = n - j;
      jA = yk + ldap1;
      b_n = yk + n;
      for (yk = 1; yk <= c; yk++) {
        b_A = ((A->data[b_n].re != 0.0) || (A->data[b_n].im != 0.0));
        if (b_A) {
          smax = -A->data[b_n].re;
          s = -A->data[b_n].im;
          ix = jp1j;
          i10 = mmj + jA;
          for (k = jA; k + 1 <= i10; k++) {
            A_re = A->data[ix - 1].re * smax - A->data[ix - 1].im * s;
            A_im = A->data[ix - 1].re * s + A->data[ix - 1].im * smax;
            A->data[k].re += A_re;
            A->data[k].im += A_im;
            ix++;
          }
        }

        b_n += n;
        jA += n;
      }
    }
  }

  b_p_size[0] = 1;
  b_p_size[1] = p_size[1];
  b_n = p_size[1] - 1;
  for (i10 = 0; i10 <= b_n; i10++) {
    b_p_data[i10] = p_data[i10];
  }

  eml_ipiv2perm(b_p_data, b_p_size, n, p_data, p_size);
  for (k = 0; k + 1 <= n; k++) {
    y->data[k + y->size[0] * (p_data[k] - 1)].re = 1.0;
    y->data[k + y->size[0] * (p_data[k] - 1)].im = 0.0;
    for (j = k; j + 1 <= n; j++) {
      b_A = ((y->data[j + y->size[0] * (p_data[k] - 1)].re != 0.0) || (y->data[j
              + y->size[0] * (p_data[k] - 1)].im != 0.0));
      if (b_A) {
        for (b_n = j + 1; b_n + 1 <= n; b_n++) {
          smax = y->data[j + y->size[0] * (p_data[k] - 1)].re * A->data[b_n +
            A->size[0] * j].re - y->data[j + y->size[0] * (p_data[k] - 1)].im *
            A->data[b_n + A->size[0] * j].im;
          s = y->data[j + y->size[0] * (p_data[k] - 1)].re * A->data[b_n +
            A->size[0] * j].im + y->data[j + y->size[0] * (p_data[k] - 1)].im *
            A->data[b_n + A->size[0] * j].re;
          y->data[b_n + y->size[0] * (p_data[k] - 1)].re -= smax;
          y->data[b_n + y->size[0] * (p_data[k] - 1)].im -= s;
        }
      }
    }
  }

  eml_xtrsm(n, n, A, n, y, n);
  emxFree_creal_T(&A);
}

void inv(const emxArray_creal_T *x, emxArray_creal_T *y)
{
  invNxN(x, y);
}

/* End of code generation (inv.c) */
