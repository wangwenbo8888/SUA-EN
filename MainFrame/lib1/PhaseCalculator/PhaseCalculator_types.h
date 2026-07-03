/*
 * PhaseCalculator_types.h
 *
 * Code generation for function 'PhaseCalculator'
 *
 * C source code generated on: Tue Mar 08 14:33:41 2016
 *
 */

#ifndef __PHASECALCULATOR_TYPES_H__
#define __PHASECALCULATOR_TYPES_H__

/* Type Definitions */
#ifndef struct_emxArray__common
#define struct_emxArray__common
typedef struct emxArray__common
{
    void *data;
    int32_T *size;
    int32_T allocatedSize;
    int32_T numDimensions;
    boolean_T canFreeData;
} emxArray__common;
#endif
#ifndef struct_emxArray_b_real_T_144
#define struct_emxArray_b_real_T_144
typedef struct emxArray_b_real_T_144
{
    real_T data[144];
    int32_T size[1];
} emxArray_b_real_T_144;
#endif
#ifndef struct_emxArray_creal_T
#define struct_emxArray_creal_T
typedef struct emxArray_creal_T
{
    creal_T *data;
    int32_T *size;
    int32_T allocatedSize;
    int32_T numDimensions;
    boolean_T canFreeData;
} emxArray_creal_T;
#endif
#ifndef struct_emxArray_creal_T_1x254
#define struct_emxArray_creal_T_1x254
typedef struct emxArray_creal_T_1x254
{
    creal_T data[254];
    int32_T size[2];
} emxArray_creal_T_1x254;
#endif
#ifndef struct_emxArray_creal_T_255
#define struct_emxArray_creal_T_255
typedef struct emxArray_creal_T_255
{
    creal_T data[255];
    int32_T size[1];
} emxArray_creal_T_255;
#endif
#ifndef struct_emxArray_int32_T_1x255
#define struct_emxArray_int32_T_1x255
typedef struct emxArray_int32_T_1x255
{
    int32_T data[255];
    int32_T size[2];
} emxArray_int32_T_1x255;
#endif
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
typedef struct emxArray_real_T
{
    real_T *data;
    int32_T *size;
    int32_T allocatedSize;
    int32_T numDimensions;
    boolean_T canFreeData;
} emxArray_real_T;
#endif
#ifndef struct_emxArray_real_T_144
#define struct_emxArray_real_T_144
typedef struct emxArray_real_T_144
{
    real_T data[144];
    int32_T size[1];
} emxArray_real_T_144;
#endif
#ifndef struct_emxArray_real_T_1x20
#define struct_emxArray_real_T_1x20
typedef struct emxArray_real_T_1x20
{
    real_T data[20];
    int32_T size[2];
} emxArray_real_T_1x20;
#endif
#ifndef struct_emxArray_real_T_1x24
#define struct_emxArray_real_T_1x24
typedef struct emxArray_real_T_1x24
{
    real_T data[24];
    int32_T size[2];
} emxArray_real_T_1x24;
#endif
#ifndef struct_emxArray_real_T_1x25
#define struct_emxArray_real_T_1x25
typedef struct emxArray_real_T_1x25
{
    real_T data[25];
    int32_T size[2];
} emxArray_real_T_1x25;
#endif
#ifndef struct_emxArray_real_T_20x20
#define struct_emxArray_real_T_20x20
typedef struct emxArray_real_T_20x20
{
    real_T data[400];
    int32_T size[2];
} emxArray_real_T_20x20;
#endif
#ifndef struct_emxArray_real_T_3x500
#define struct_emxArray_real_T_3x500
typedef struct emxArray_real_T_3x500
{
    real_T data[1500];
    int32_T size[2];
} emxArray_real_T_3x500;
#endif
#ifndef struct_emxArray_real_T_400
#define struct_emxArray_real_T_400
typedef struct emxArray_real_T_400
{
    real_T data[400];
    int32_T size[1];
} emxArray_real_T_400;
#endif
#ifndef struct_emxArray_real_T_400x1
#define struct_emxArray_real_T_400x1
typedef struct emxArray_real_T_400x1
{
    real_T data[400];
    int32_T size[2];
} emxArray_real_T_400x1;
#endif
#ifndef struct_emxArray_real_T_500
#define struct_emxArray_real_T_500
typedef struct emxArray_real_T_500
{
    real_T data[500];
    int32_T size[1];
} emxArray_real_T_500;
#endif
#ifndef struct_emxArray_real_T_500x1
#define struct_emxArray_real_T_500x1
typedef struct emxArray_real_T_500x1
{
    real_T data[500];
    int32_T size[2];
} emxArray_real_T_500x1;
#endif
#ifndef struct_emxArray_real_T_500x3
#define struct_emxArray_real_T_500x3
typedef struct emxArray_real_T_500x3
{
    real_T data[1500];
    int32_T size[2];
} emxArray_real_T_500x3;
#endif
typedef struct
{
    real_T density;
    real_T frequency;
    real_T velocity;
    real_T attenuation;
    real_T wavenumber;
} struct_T;

#endif
/* End of code generation (PhaseCalculator_types.h) */
