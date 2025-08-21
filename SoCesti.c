/*
 * SoCesti.c
 *
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * Code generation for model "SoCesti".
 *
 * Model version              : 1.4
 * Simulink Coder version : 25.1 (R2025a) 21-Nov-2024
 * C source code generated on : Thu Aug 21 10:27:48 2025
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "SoCesti.h"
#include <string.h>
#include <emmintrin.h>
#include <math.h>
#include "rtwtypes.h"
#include "SoCesti_private.h"

/* Block states (default storage) */
DW_SoCesti_T SoCesti_DW;

/* External inputs (root inport signals with default storage) */
ExtU_SoCesti_T SoCesti_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_SoCesti_T SoCesti_Y;

/* Real-time model */
static RT_MODEL_SoCesti_T SoCesti_M_;
RT_MODEL_SoCesti_T *const SoCesti_M = &SoCesti_M_;
real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex)
{
  real_T frac;
  real_T yL_0d0;
  uint32_T iLeft;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    uint32_T bpIdx;
    uint32_T iRght;

    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  yL_0d0 = table[iLeft];
  return (table[iLeft + 1U] - yL_0d0) * frac + yL_0d0;
}

/* Model step function */
void SoCesti_step(void)
{
  __m128d tmp_0;
  __m128d tmp_1;
  __m128d tmp_2;
  real_T rtb_gain_0[9];
  real_T tmp[9];
  real_T rtb_Add_h[4];
  real_T rtb_Add_l[4];
  real_T rtb_Reshape[3];
  real_T rtb_gain[3];
  real_T tmp_3[2];
  real_T Delay_DSTATE_f;
  real_T Delay_DSTATE_f_0;
  real_T Delay_DSTATE_f_1;
  real_T Delay_DSTATE_f_2;
  real_T rtb_Add2;
  real_T rtb_Add_c_idx_0;
  real_T rtb_Add_c_idx_1;
  real_T rtb_Saturation1;
  real_T rtb_Saturation2;
  real_T rtb_Saturation_h;
  int32_T i;
  int32_T i_0;
  int32_T tmp_4;

  /* Delay: '<S19>/Delay' incorporates:
   *  Constant: '<S17>/Constant2'
   */
  if (SoCesti_DW.icLoad) {
    memcpy(&SoCesti_DW.Delay_DSTATE_o[0], &SoCesti_ConstP.Constant2_Value[0], 9U
           * sizeof(real_T));
  }

  /* Reshape: '<S21>/Reshape' incorporates:
   *  Delay: '<S21>/Delay'
   *  Delay: '<S21>/Delay1'
   *  Delay: '<S21>/Delay2'
   *  Inport: '<Root>/current'
   *  Sum: '<S21>/Sum'
   */
  rtb_Reshape[0] = SoCesti_DW.Delay2_DSTATE - SoCesti_DW.Delay1_DSTATE;
  rtb_Reshape[1] = SoCesti_U.current;
  rtb_Reshape[2] = SoCesti_DW.Delay_DSTATE_m;

  /* MATLAB Function: '<S20>/MATLAB Function' incorporates:
   *  Constant: '<S17>/Constant'
   *  Delay: '<S20>/Delay'
   *  Delay: '<S21>/Delay'
   *  Inport: '<Root>/current'
   *  Math: '<S27>/Math Function'
   *  Reshape: '<S21>/Reshape'
   */
  rtb_Saturation1 = 0.0;
  for (i = 0; i < 3; i++) {
    rtb_Saturation1 += ((SoCesti_DW.Delay_DSTATE[3 * i + 1] * SoCesti_U.current
                         + SoCesti_DW.Delay_DSTATE[3 * i] * rtb_Reshape[0]) +
                        SoCesti_DW.Delay_DSTATE[3 * i + 2] *
                        SoCesti_DW.Delay_DSTATE_m) * rtb_Reshape[i];
  }

  for (i = 0; i <= 0; i += 2) {
    tmp_0 = _mm_loadu_pd(&SoCesti_DW.Delay_DSTATE[i + 3]);
    tmp_1 = _mm_loadu_pd(&SoCesti_DW.Delay_DSTATE[i]);
    tmp_2 = _mm_loadu_pd(&SoCesti_DW.Delay_DSTATE[i + 6]);
    _mm_storeu_pd(&rtb_gain[i], _mm_div_pd(_mm_add_pd(_mm_add_pd(_mm_mul_pd
      (tmp_0, _mm_set1_pd(SoCesti_U.current)), _mm_mul_pd(tmp_1, _mm_set1_pd
      (rtb_Reshape[0]))), _mm_mul_pd(tmp_2, _mm_set1_pd
      (SoCesti_DW.Delay_DSTATE_m))), _mm_set1_pd(rtb_Saturation1 + 0.9999)));
  }

  for (i = 2; i < 3; i++) {
    rtb_gain[i] = ((SoCesti_DW.Delay_DSTATE[i + 3] * SoCesti_U.current +
                    SoCesti_DW.Delay_DSTATE[i] * rtb_Reshape[0]) +
                   SoCesti_DW.Delay_DSTATE[i + 6] * SoCesti_DW.Delay_DSTATE_m) /
      (rtb_Saturation1 + 0.9999);
  }

  /* End of MATLAB Function: '<S20>/MATLAB Function' */

  /* MATLAB Function: '<S19>/MATLAB Function' incorporates:
   *  Delay: '<S19>/Delay'
   *  Math: '<S25>/Math Function'
   *  Reshape: '<S21>/Reshape'
   */
  rtb_Saturation1 = rtb_gain[0];
  rtb_Saturation2 = rtb_gain[1];
  rtb_Saturation_h = rtb_gain[2];
  for (i = 0; i < 3; i++) {
    _mm_storeu_pd(&rtb_gain_0[3 * i], _mm_mul_pd(_mm_set_pd(rtb_Saturation2,
      rtb_Saturation1), _mm_set1_pd(rtb_Reshape[i])));
    rtb_gain_0[3 * i + 2] = rtb_Saturation_h * rtb_Reshape[i];
  }

  for (i = 0; i < 3; i++) {
    rtb_Saturation1 = rtb_gain_0[i + 3];
    rtb_Saturation2 = rtb_gain_0[i];
    rtb_Saturation_h = rtb_gain_0[i + 6];
    for (i_0 = 0; i_0 < 3; i_0++) {
      tmp_4 = 3 * i_0 + i;
      tmp[tmp_4] = SoCesti_DW.Delay_DSTATE_o[tmp_4] -
        ((SoCesti_DW.Delay_DSTATE_o[3 * i_0 + 1] * rtb_Saturation1 +
          SoCesti_DW.Delay_DSTATE_o[3 * i_0] * rtb_Saturation2) +
         SoCesti_DW.Delay_DSTATE_o[3 * i_0 + 2] * rtb_Saturation_h);
    }
  }

  for (i = 0; i <= 6; i += 2) {
    tmp_0 = _mm_loadu_pd(&tmp[i]);
    _mm_storeu_pd(&SoCesti_DW.Delay_DSTATE_o[i], _mm_mul_pd(_mm_set1_pd
      (1.000100010001), tmp_0));
  }

  for (i = 8; i < 9; i++) {
    SoCesti_DW.Delay_DSTATE_o[i] = 1.000100010001 * tmp[i];
  }

  /* End of MATLAB Function: '<S19>/MATLAB Function' */

  /* Delay: '<S6>/Delay' incorporates:
   *  Constant: '<S6>/Constant'
   *  Product: '<S13>/Matrix Multiply'
   */
  if (SoCesti_DW.icLoad_h) {
    SoCesti_DW.Delay_DSTATE_f[0] = 1.0;
    SoCesti_DW.Delay_DSTATE_f[1] = 0.0;
    SoCesti_DW.Delay_DSTATE_f[2] = 0.0;
    SoCesti_DW.Delay_DSTATE_f[3] = 1.0;
  }

  /* Delay: '<S22>/Delay' incorporates:
   *  Constant: '<S17>/Constant1'
   */
  if (SoCesti_DW.icLoad_p) {
    SoCesti_DW.Delay_DSTATE_p[0] = 0.0016;
    SoCesti_DW.Delay_DSTATE_p[1] = 0.0063;
    SoCesti_DW.Delay_DSTATE_p[2] = 0.0013;
  }

  /* MATLAB Function: '<S22>/MATLAB Function' incorporates:
   *  Delay: '<S17>/Delay'
   *  Delay: '<S21>/Delay'
   *  Delay: '<S22>/Delay'
   *  Inport: '<Root>/current'
   *  Math: '<S29>/Math Function'
   *  Reshape: '<S21>/Reshape'
   */
  rtb_Saturation1 = SoCesti_DW.Delay_DSTATE_g - ((rtb_Reshape[0] *
    SoCesti_DW.Delay_DSTATE_p[0] + SoCesti_U.current *
    SoCesti_DW.Delay_DSTATE_p[1]) + SoCesti_DW.Delay_DSTATE_m *
    SoCesti_DW.Delay_DSTATE_p[2]);
  tmp_0 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&rtb_gain[0]), _mm_set1_pd
    (rtb_Saturation1)), _mm_loadu_pd(&SoCesti_DW.Delay_DSTATE_p[0]));
  _mm_storeu_pd(&SoCesti_DW.Delay_DSTATE_p[0], tmp_0);
  SoCesti_DW.Delay_DSTATE_p[2] += rtb_gain[2] * rtb_Saturation1;

  /* Saturate: '<S17>/Saturation' */
  if (SoCesti_DW.Delay_DSTATE_p[0] > 0.1) {
    rtb_Saturation_h = 0.1;
  } else if (SoCesti_DW.Delay_DSTATE_p[0] < 1.0E-6) {
    rtb_Saturation_h = 1.0E-6;
  } else {
    rtb_Saturation_h = SoCesti_DW.Delay_DSTATE_p[0];
  }

  /* End of Saturate: '<S17>/Saturation' */

  /* Saturate: '<S17>/Saturation1' */
  if (SoCesti_DW.Delay_DSTATE_p[1] > 0.5) {
    rtb_Saturation1 = 0.5;
  } else if (SoCesti_DW.Delay_DSTATE_p[1] < 1.0E-6) {
    rtb_Saturation1 = 1.0E-6;
  } else {
    rtb_Saturation1 = SoCesti_DW.Delay_DSTATE_p[1];
  }

  /* End of Saturate: '<S17>/Saturation1' */

  /* Saturate: '<S17>/Saturation2' */
  if (SoCesti_DW.Delay_DSTATE_p[2] > 0.5) {
    rtb_Saturation2 = 0.5;
  } else if (SoCesti_DW.Delay_DSTATE_p[2] < 1.0E-6) {
    rtb_Saturation2 = 1.0E-6;
  } else {
    rtb_Saturation2 = SoCesti_DW.Delay_DSTATE_p[2];
  }

  /* End of Saturate: '<S17>/Saturation2' */

  /* MATLAB Function: '<S17>/MATLAB Function' */
  rtb_Add2 = rtb_Saturation_h * rtb_Saturation1 + rtb_Saturation2;
  rtb_Saturation2 = rtb_Add2 / (1.0 - rtb_Saturation_h);

  /* Math: '<S11>/Math Function' incorporates:
   *  Constant: '<S11>/Constant'
   *  Constant: '<S4>/sampling time [s]'
   *  MATLAB Function: '<S17>/MATLAB Function'
   *  Product: '<S11>/Divide'
   *
   * About '<S11>/Math Function':
   *  Operator: exp
   */
  rtb_Saturation_h = exp(-1.0 / rtb_Saturation2 / (-(1.0 - rtb_Saturation_h) /
    (rtb_Add2 * log(rtb_Saturation_h))));

  /* Product: '<S9>/Product' incorporates:
   *  Constant: '<S9>/Constant'
   */
  rtb_Add2 = 0.0 * rtb_Saturation_h;

  /* Sum: '<S9>/Add' incorporates:
   *  Constant: '<S9>/Constant1'
   *  Product: '<S9>/Product'
   */
  rtb_Add_h[0] = rtb_Add2 + 1.0;
  rtb_Add_h[1] = rtb_Add2;
  rtb_Add_h[2] = rtb_Add2;
  rtb_Add_h[3] = rtb_Saturation_h;

  /* Product: '<S14>/Matrix Multiply' incorporates:
   *  Delay: '<S6>/Delay'
   *  Math: '<S16>/Math Function'
   *  Sum: '<S9>/Add'
   */
  Delay_DSTATE_f = SoCesti_DW.Delay_DSTATE_f[0];
  Delay_DSTATE_f_0 = SoCesti_DW.Delay_DSTATE_f[1];
  Delay_DSTATE_f_1 = SoCesti_DW.Delay_DSTATE_f[2];
  Delay_DSTATE_f_2 = SoCesti_DW.Delay_DSTATE_f[3];
  for (i = 0; i < 2; i++) {
    rtb_Add2 = rtb_Add_h[i];
    rtb_Add_c_idx_0 = Delay_DSTATE_f * rtb_Add2;
    rtb_Add_c_idx_1 = Delay_DSTATE_f_0 * rtb_Add2;
    rtb_Add2 = rtb_Add_h[i + 2];
    i_0 = i << 1;
    rtb_Add_l[i_0] = Delay_DSTATE_f_1 * rtb_Add2 + rtb_Add_c_idx_0;
    rtb_Add_l[i_0 + 1] = Delay_DSTATE_f_2 * rtb_Add2 + rtb_Add_c_idx_1;
  }

  rtb_Add2 = rtb_Add_l[1];
  rtb_Add_c_idx_0 = rtb_Add_l[0];
  rtb_Add_c_idx_1 = rtb_Add_l[3];
  Delay_DSTATE_f = rtb_Add_l[2];

  /* Sum: '<S14>/Add' incorporates:
   *  Constant: '<S5>/Q'
   *  Product: '<S14>/Matrix Multiply'
   *  Sum: '<S9>/Add'
   */
  for (i = 0; i <= 0; i += 2) {
    tmp_0 = _mm_loadu_pd(&rtb_Add_h[i + 2]);
    tmp_1 = _mm_loadu_pd(&rtb_Add_h[i]);
    _mm_storeu_pd(&rtb_Add_l[i], _mm_add_pd(_mm_add_pd(_mm_mul_pd(tmp_0,
      _mm_set1_pd(rtb_Add2)), _mm_mul_pd(tmp_1, _mm_set1_pd(rtb_Add_c_idx_0))),
      _mm_loadu_pd(&SoCesti_ConstP.Q_Value[i])));
    _mm_storeu_pd(&rtb_Add_l[i + 2], _mm_add_pd(_mm_add_pd(_mm_mul_pd(tmp_0,
      _mm_set1_pd(rtb_Add_c_idx_1)), _mm_mul_pd(tmp_1, _mm_set1_pd
      (Delay_DSTATE_f))), _mm_loadu_pd(&SoCesti_ConstP.Q_Value[i + 2])));
  }

  /* End of Sum: '<S14>/Add' */

  /* Delay: '<S8>/Delay' incorporates:
   *  Constant: '<S8>/Constant1'
   */
  if (SoCesti_DW.icLoad_pj) {
    SoCesti_DW.Delay_DSTATE_k[0] = 1.0;
    SoCesti_DW.Delay_DSTATE_k[1] = 0.0;
  }

  /* Lookup_n-D: '<S2>/1-D Lookup Table4' incorporates:
   *  Delay: '<S8>/Delay'
   */
  rtb_Add2 = look1_binlxpw(SoCesti_DW.Delay_DSTATE_k[0], SoCesti_ConstP.pooled5,
    SoCesti_ConstP.uDLookupTable4_tableData, 200U);

  /* Sum: '<S10>/Add' incorporates:
   *  Constant: '<S10>/Constant'
   *  Constant: '<S10>/Constant1'
   *  Product: '<S10>/Product'
   */
  rtb_Add_c_idx_0 = rtb_Add2;
  rtb_Add_c_idx_1 = 0.0 * rtb_Add2 - 1.0;

  /* Product: '<S12>/Matrix Multiply' incorporates:
   *  Math: '<S15>/Math Function'
   *  Product: '<S10>/Product'
   *  Product: '<S13>/Matrix Multiply'
   *  Sum: '<S10>/Add'
   *  Sum: '<S14>/Add'
   */
  Delay_DSTATE_f_0 = rtb_Add_l[0] * rtb_Add2;
  Delay_DSTATE_f = rtb_Add_l[3] * rtb_Add_c_idx_1;

  /* Math: '<S12>/Math Function' incorporates:
   *  Constant: '<S5>/Constant2'
   *  Math: '<S15>/Math Function'
   *  Product: '<S10>/Product'
   *  Product: '<S12>/Matrix Multiply'
   *  Sum: '<S10>/Add'
   *  Sum: '<S12>/Add'
   *  Sum: '<S14>/Add'
   *
   * About '<S12>/Math Function':
   *  Operator: reciprocal
   */
  rtb_Add2 = 1.0 / (((rtb_Add_l[2] * rtb_Add_c_idx_1 + Delay_DSTATE_f_0) *
                     rtb_Add2 + (rtb_Add_l[1] * rtb_Add2 + Delay_DSTATE_f) *
                     rtb_Add_c_idx_1) + 1.0);

  /* Product: '<S12>/Matrix Multiply1' incorporates:
   *  Math: '<S15>/Math Function'
   *  Sum: '<S10>/Add'
   *  Sum: '<S14>/Add'
   */
  tmp_0 = _mm_mul_pd(_mm_loadu_pd(&rtb_Add_l[0]), _mm_set1_pd(rtb_Add_c_idx_0 *
    rtb_Add2));
  _mm_storeu_pd(&tmp_3[0], tmp_0);

  /* Product: '<S13>/Matrix Multiply' incorporates:
   *  Product: '<S12>/Matrix Multiply1'
   *  Sum: '<S10>/Add'
   *  Sum: '<S14>/Add'
   */
  Delay_DSTATE_f_0 += rtb_Add_c_idx_1 * rtb_Add_l[1];

  /* Product: '<S12>/Matrix Multiply1' incorporates:
   *  Math: '<S15>/Math Function'
   *  Sum: '<S10>/Add'
   *  Sum: '<S14>/Add'
   */
  tmp_0 = _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(&rtb_Add_l[2]), _mm_set1_pd
    (rtb_Add_c_idx_1 * rtb_Add2)), _mm_set_pd(tmp_3[1], tmp_3[0]));
  _mm_storeu_pd(&tmp_3[0], tmp_0);

  /* Product: '<S13>/Matrix Multiply' incorporates:
   *  Product: '<S12>/Matrix Multiply1'
   *  Sum: '<S10>/Add'
   *  Sum: '<S14>/Add'
   */
  rtb_Add2 = rtb_Add_c_idx_0 * rtb_Add_l[2] + Delay_DSTATE_f;
  SoCesti_DW.Delay_DSTATE_f[0] = tmp_3[0] * Delay_DSTATE_f_0;
  SoCesti_DW.Delay_DSTATE_f[1] = tmp_3[1] * Delay_DSTATE_f_0;
  SoCesti_DW.Delay_DSTATE_f[2] = tmp_3[0] * rtb_Add2;
  SoCesti_DW.Delay_DSTATE_f[3] = tmp_3[1] * rtb_Add2;

  /* Sum: '<S8>/Add2' incorporates:
   *  Delay: '<S8>/Delay'
   *  Inport: '<Root>/current'
   *  Product: '<S8>/Divide1'
   *  Product: '<S8>/Divide2'
   *  Product: '<S8>/Divide3'
   *  Sum: '<S8>/Add1'
   */
  rtb_Add2 = (rtb_Saturation2 - rtb_Saturation_h * rtb_Saturation2) *
    SoCesti_U.current + SoCesti_DW.Delay_DSTATE_k[1] * rtb_Saturation_h;

  /* Lookup_n-D: '<S2>/1-D Lookup Table1' incorporates:
   *  Delay: '<S8>/Delay'
   */
  SoCesti_DW.Delay2_DSTATE = look1_binlxpw(SoCesti_DW.Delay_DSTATE_k[0],
    SoCesti_ConstP.pooled5, SoCesti_ConstP.uDLookupTable1_tableData, 200U);

  /* Sum: '<S1>/Sum1' incorporates:
   *  Inport: '<Root>/current'
   *  Inport: '<Root>/voltage'
   *  MATLAB Function: '<S17>/MATLAB Function'
   *  Product: '<S7>/Product'
   *  Sum: '<S7>/Add'
   */
  rtb_Saturation1 = SoCesti_U.voltage - ((SoCesti_DW.Delay2_DSTATE - rtb_Add2) -
    SoCesti_U.current * rtb_Saturation1);

  /* Sum: '<S1>/Sum' incorporates:
   *  Constant: '<S4>/[C]'
   *  Constant: '<S4>/sampling time [s]'
   *  Delay: '<S8>/Delay'
   *  Inport: '<Root>/current'
   *  Product: '<S6>/Product'
   *  Product: '<S8>/Divide'
   *  Sum: '<S8>/Add'
   */
  rtb_Saturation2 = (SoCesti_DW.Delay_DSTATE_k[0] - 5.41013122814307E-5 *
                     SoCesti_U.current) + rtb_Saturation1 * tmp_3[0];

  /* Outport: '<Root>/SoC' incorporates:
   *  Delay: '<S8>/Delay'
   */
  SoCesti_Y.SoC = SoCesti_DW.Delay_DSTATE_k[0];

  /* Update for Delay: '<S20>/Delay' */
  for (i = 0; i < 9; i++) {
    /* Saturate: '<S19>/Saturation' */
    rtb_Saturation_h = SoCesti_DW.Delay_DSTATE_o[i];
    if (rtb_Saturation_h <= 1.0E-6) {
      SoCesti_DW.Delay_DSTATE[i] = 1.0E-6;
    } else {
      SoCesti_DW.Delay_DSTATE[i] = rtb_Saturation_h;
    }

    /* End of Saturate: '<S19>/Saturation' */
  }

  /* End of Update for Delay: '<S20>/Delay' */

  /* Update for Delay: '<S19>/Delay' */
  SoCesti_DW.icLoad = false;

  /* Update for Delay: '<S21>/Delay1' incorporates:
   *  Inport: '<Root>/voltage'
   */
  SoCesti_DW.Delay1_DSTATE = SoCesti_U.voltage;

  /* Update for Delay: '<S21>/Delay' incorporates:
   *  Inport: '<Root>/current'
   */
  SoCesti_DW.Delay_DSTATE_m = SoCesti_U.current;

  /* Update for Delay: '<S6>/Delay' */
  SoCesti_DW.icLoad_h = false;

  /* Sum: '<S13>/Subtract' incorporates:
   *  Product: '<S13>/Matrix Multiply'
   *  Sum: '<S14>/Add'
   */
  tmp_0 = _mm_sub_pd(_mm_loadu_pd(&rtb_Add_l[0]), _mm_loadu_pd
                     (&SoCesti_DW.Delay_DSTATE_f[0]));

  /* Update for Product: '<S13>/Matrix Multiply' incorporates:
   *  Delay: '<S6>/Delay'
   *  Sum: '<S13>/Subtract'
   *  Sum: '<S14>/Add'
   */
  _mm_storeu_pd(&SoCesti_DW.Delay_DSTATE_f[0], tmp_0);

  /* Sum: '<S13>/Subtract' incorporates:
   *  Product: '<S13>/Matrix Multiply'
   *  Sum: '<S14>/Add'
   */
  tmp_0 = _mm_sub_pd(_mm_loadu_pd(&rtb_Add_l[2]), _mm_loadu_pd
                     (&SoCesti_DW.Delay_DSTATE_f[2]));

  /* Update for Product: '<S13>/Matrix Multiply' incorporates:
   *  Delay: '<S6>/Delay'
   *  Sum: '<S13>/Subtract'
   *  Sum: '<S14>/Add'
   */
  _mm_storeu_pd(&SoCesti_DW.Delay_DSTATE_f[2], tmp_0);

  /* Update for Delay: '<S17>/Delay' incorporates:
   *  Inport: '<Root>/current'
   *  Math: '<S30>/Math Function'
   *  Product: '<S23>/Matrix Multiply'
   *  Reshape: '<S21>/Reshape'
   */
  SoCesti_DW.Delay_DSTATE_g = (rtb_Reshape[0] * SoCesti_DW.Delay_DSTATE_p[0] +
    SoCesti_U.current * SoCesti_DW.Delay_DSTATE_p[1]) + rtb_Reshape[2] *
    SoCesti_DW.Delay_DSTATE_p[2];

  /* Update for Delay: '<S22>/Delay' */
  SoCesti_DW.icLoad_p = false;

  /* Update for Delay: '<S8>/Delay' */
  SoCesti_DW.icLoad_pj = false;

  /* Saturate: '<S1>/Saturation' */
  if (rtb_Saturation2 > 1.0) {
    /* Update for Delay: '<S8>/Delay' */
    SoCesti_DW.Delay_DSTATE_k[0] = 1.0;
  } else if (rtb_Saturation2 < 0.0) {
    /* Update for Delay: '<S8>/Delay' */
    SoCesti_DW.Delay_DSTATE_k[0] = 0.0;
  } else {
    /* Update for Delay: '<S8>/Delay' */
    SoCesti_DW.Delay_DSTATE_k[0] = rtb_Saturation2;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Update for Delay: '<S8>/Delay' incorporates:
   *  Product: '<S6>/Product'
   *  Reshape: '<S1>/Reshape'
   *  Sum: '<S1>/Sum'
   */
  SoCesti_DW.Delay_DSTATE_k[1] = rtb_Saturation1 * tmp_3[1] + rtb_Add2;

  /* Matfile logging */
  rt_UpdateTXYLogVars(SoCesti_M->rtwLogInfo, (&SoCesti_M->Timing.taskTime0));

  /* signal main to stop simulation */
  {                                    /* Sample time: [1.0s, 0.0s] */
    if ((rtmGetTFinal(SoCesti_M)!=-1) &&
        !((rtmGetTFinal(SoCesti_M)-SoCesti_M->Timing.taskTime0) >
          SoCesti_M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(SoCesti_M, "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++SoCesti_M->Timing.clockTick0)) {
    ++SoCesti_M->Timing.clockTickH0;
  }

  SoCesti_M->Timing.taskTime0 = SoCesti_M->Timing.clockTick0 *
    SoCesti_M->Timing.stepSize0 + SoCesti_M->Timing.clockTickH0 *
    SoCesti_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void SoCesti_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)SoCesti_M, 0,
                sizeof(RT_MODEL_SoCesti_T));
  rtmSetTFinal(SoCesti_M, 37660.0);
  SoCesti_M->Timing.stepSize0 = 1.0;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    SoCesti_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(SoCesti_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(SoCesti_M->rtwLogInfo, (NULL));
    rtliSetLogT(SoCesti_M->rtwLogInfo, "tout");
    rtliSetLogX(SoCesti_M->rtwLogInfo, "");
    rtliSetLogXFinal(SoCesti_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(SoCesti_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(SoCesti_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(SoCesti_M->rtwLogInfo, 0);
    rtliSetLogDecimation(SoCesti_M->rtwLogInfo, 1);
    rtliSetLogY(SoCesti_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(SoCesti_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(SoCesti_M->rtwLogInfo, (NULL));
  }

  /* states (dwork) */
  (void) memset((void *)&SoCesti_DW, 0,
                sizeof(DW_SoCesti_T));

  /* external inputs */
  (void)memset(&SoCesti_U, 0, sizeof(ExtU_SoCesti_T));

  /* external outputs */
  SoCesti_Y.SoC = 0.0;

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(SoCesti_M->rtwLogInfo, 0.0, rtmGetTFinal
    (SoCesti_M), SoCesti_M->Timing.stepSize0, (&rtmGetErrorStatus(SoCesti_M)));

  /* InitializeConditions for Delay: '<S20>/Delay' */
  memset(&SoCesti_DW.Delay_DSTATE[0], 0, 9U * sizeof(real_T));

  /* InitializeConditions for Delay: '<S19>/Delay' */
  SoCesti_DW.icLoad = true;

  /* InitializeConditions for Delay: '<S21>/Delay1' */
  SoCesti_DW.Delay1_DSTATE = 0.0;

  /* InitializeConditions for Lookup_n-D: '<S2>/1-D Lookup Table1' incorporates:
   *  Delay: '<S21>/Delay2'
   */
  SoCesti_DW.Delay2_DSTATE = 0.0;

  /* InitializeConditions for Delay: '<S21>/Delay' */
  SoCesti_DW.Delay_DSTATE_m = 0.0;

  /* InitializeConditions for Delay: '<S6>/Delay' */
  SoCesti_DW.icLoad_h = true;

  /* InitializeConditions for Delay: '<S17>/Delay' */
  SoCesti_DW.Delay_DSTATE_g = 0.0;

  /* InitializeConditions for Delay: '<S22>/Delay' */
  SoCesti_DW.icLoad_p = true;

  /* InitializeConditions for Delay: '<S8>/Delay' */
  SoCesti_DW.icLoad_pj = true;
}

/* Model terminate function */
void SoCesti_terminate(void)
{
  /* (no terminate code required) */
}
