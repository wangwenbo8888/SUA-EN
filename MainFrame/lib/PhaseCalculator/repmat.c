/*
 * repmat.c
 *
 * Code generation for function 'repmat'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "PhaseCalculator.h"
#include "repmat.h"
#include "psuedo_inverse.h"
#include "PhaseCalculator_rtwutil.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
static int32_T mul_s32_s32_s32_sat(int32_T a, int32_T b);
static void mul_wide_s32(int32_T in0, int32_T in1, uint32_T *ptrOutBitsHi,
  uint32_T *ptrOutBitsLo);

/* Function Definitions */
static int32_T mul_s32_s32_s32_sat(int32_T a, int32_T b)
{
  int32_T result;
  uint32_T u32_clo;
  uint32_T u32_chi;
  mul_wide_s32(a, b, &u32_chi, &u32_clo);
  if (((int32_T)u32_chi > 0) || ((u32_chi == 0U) && (u32_clo >= 2147483648U))) {
    result = MAX_int32_T;
  } else if (((int32_T)u32_chi < -1) || (((int32_T)u32_chi == -1) && (u32_clo <
               2147483648U))) {
    result = MIN_int32_T;
  } else {
    result = (int32_T)u32_clo;
  }

  return result;
}

static void mul_wide_s32(int32_T in0, int32_T in1, uint32_T *ptrOutBitsHi,
  uint32_T *ptrOutBitsLo)
{
  uint32_T absIn0;
  uint32_T absIn1;
  int32_T negativeProduct;
  int32_T in0Hi;
  int32_T in0Lo;
  int32_T in1Hi;
  int32_T in1Lo;
  uint32_T productLoHi;
  uint32_T productLoLo;
  uint32_T outBitsLo;
  absIn0 = (uint32_T)(in0 < 0 ? -in0 : in0);
  absIn1 = (uint32_T)(in1 < 0 ? -in1 : in1);
  negativeProduct = !((in0 == 0) || ((in1 == 0) || ((in0 > 0) == (in1 > 0))));
  in0Hi = (int32_T)(absIn0 >> 16U);
  in0Lo = (int32_T)(absIn0 & 65535U);
  in1Hi = (int32_T)(absIn1 >> 16U);
  in1Lo = (int32_T)(absIn1 & 65535U);
  absIn0 = (uint32_T)in0Hi * (uint32_T)in1Hi;
  absIn1 = (uint32_T)in0Hi * (uint32_T)in1Lo;
  productLoHi = (uint32_T)in0Lo * (uint32_T)in1Hi;
  productLoLo = (uint32_T)in0Lo * (uint32_T)in1Lo;
  in0Hi = 0;
  outBitsLo = productLoLo + (productLoHi << 16U);
  if (outBitsLo < productLoLo) {
    in0Hi = 1;
  }

  productLoLo = outBitsLo;
  outBitsLo += absIn1 << 16U;
  if (outBitsLo < productLoLo) {
    in0Hi = (int32_T)((uint32_T)in0Hi + 1U);
  }

  absIn0 = (((uint32_T)in0Hi + absIn0) + (productLoHi >> 16U)) + (absIn1 >> 16U);
  if (negativeProduct) {
    absIn0 = ~absIn0;
    outBitsLo = ~outBitsLo;
    outBitsLo++;
    if (outBitsLo == 0U) {
      absIn0++;
    }
  }

  *ptrOutBitsHi = absIn0;
  *ptrOutBitsLo = outBitsLo;
}

void b_repmat(const real_T a[3], real_T m, real_T b_data[1500], int32_T b_size[2])
{
  real_T d1;
  int32_T ia;
  int32_T mv[2];
  int32_T outsize[2];
  int32_T ib;
  int32_T iacol;
  int32_T jcol;
  int32_T itilerow;
  d1 = rt_roundd_snf(m);
  if (d1 < 2.147483648E+9) {
    if (d1 >= -2.147483648E+9) {
      ia = (int32_T)d1;
    } else {
      ia = MIN_int32_T;
    }
  } else if (d1 >= 2.147483648E+9) {
    ia = MAX_int32_T;
  } else {
    ia = 0;
  }

  mv[0] = ia;
  mv[1] = 1;
  for (ia = 0; ia < 2; ia++) {
    outsize[ia] = mul_s32_s32_s32_sat(1 + (ia << 1), mv[ia]);
  }

  b_size[0] = outsize[0];
  b_size[1] = 3;
  if (b_size[0] == 0) {
  } else {
    ia = 1;
    ib = 0;
    iacol = 1;
    for (jcol = 0; jcol < 3; jcol++) {
      for (itilerow = 1; itilerow <= mv[0]; itilerow++) {
        b_data[ib] = a[iacol - 1];
        ia = iacol + 1;
        ib++;
      }

      iacol = ia;
    }
  }
}

void c_repmat(real_T a, uint8_T m, real_T b_data[255], int32_T b_size[1])
{
  uint8_T b_m[2];
  uint8_T outsize[2];
  int32_T i9;
  int32_T loop_ub;
  b_m[0] = m;
  b_m[1] = 1;
  for (i9 = 0; i9 < 2; i9++) {
    outsize[i9] = b_m[i9];
  }

  b_size[0] = outsize[0];
  loop_ub = outsize[0] - 1;
  for (i9 = 0; i9 <= loop_ub; i9++) {
    b_data[i9] = 1.0904850535426885E+6;
  }
}

void repmat(const real_T a[3], real_T n, real_T b_data[1500], int32_T b_size[2])
{
  int32_T mv[2];
  real_T d0;
  int32_T ib;
  int32_T outsize[2];
  int32_T jtilecol;
  int32_T ia;
  int32_T k;
  mv[0] = 1;
  d0 = rt_roundd_snf(n);
  if (d0 < 2.147483648E+9) {
    if (d0 >= -2.147483648E+9) {
      ib = (int32_T)d0;
    } else {
      ib = MIN_int32_T;
    }
  } else if (d0 >= 2.147483648E+9) {
    ib = MAX_int32_T;
  } else {
    ib = 0;
  }

  mv[1] = ib;
  for (ib = 0; ib < 2; ib++) {
    outsize[ib] = mul_s32_s32_s32_sat(3 + -2 * ib, mv[ib]);
  }

  b_size[0] = 3;
  b_size[1] = outsize[1];
  if (b_size[1] == 0) {
  } else {
    ib = 0;
    for (jtilecol = 1; jtilecol <= mv[1]; jtilecol++) {
      ia = 0;
      for (k = 0; k < 3; k++) {
        b_data[ib] = a[ia];
        ia++;
        ib++;
      }
    }
  }
}

/* End of code generation (repmat.c) */
