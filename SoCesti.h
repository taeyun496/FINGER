/*
 * SoCesti.h
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

#ifndef SoCesti_h_
#define SoCesti_h_
#ifndef SoCesti_COMMON_INCLUDES_
#define SoCesti_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* SoCesti_COMMON_INCLUDES_ */

#include "SoCesti_types.h"
#include <float.h>
#include <string.h>
#include <stddef.h>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   ((rtm)->Timing.taskTime0)
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                (&(rtm)->Timing.taskTime0)
#endif

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Delay_DSTATE[9];              /* '<S20>/Delay' */
  real_T Delay_DSTATE_o[9];            /* '<S19>/Delay' */
  real_T Delay1_DSTATE;                /* '<S21>/Delay1' */
  real_T Delay2_DSTATE;                /* '<S21>/Delay2' */
  real_T Delay_DSTATE_m;               /* '<S21>/Delay' */
  real_T Delay_DSTATE_f[4];            /* '<S6>/Delay' */
  real_T Delay_DSTATE_g;               /* '<S17>/Delay' */
  real_T Delay_DSTATE_p[3];            /* '<S22>/Delay' */
  real_T Delay_DSTATE_k[2];            /* '<S8>/Delay' */
  boolean_T icLoad;                    /* '<S19>/Delay' */
  boolean_T icLoad_h;                  /* '<S6>/Delay' */
  boolean_T icLoad_p;                  /* '<S22>/Delay' */
  boolean_T icLoad_pj;                 /* '<S8>/Delay' */
} DW_SoCesti_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: [1e-9,0;0, 1]
   * Referenced by: '<S5>/Q'
   */
  real_T Q_Value[4];

  /* Expression: 1*eye(3)
   * Referenced by: '<S17>/Constant2'
   */
  real_T Constant2_Value[9];

  /* Expression: dOCV_dSOC_25
   * Referenced by: '<S2>/1-D Lookup Table4'
   */
  real_T uDLookupTable4_tableData[201];

  /* Pooled Parameter (Expression: SOC)
   * Referenced by:
   *   '<S2>/1-D Lookup Table1'
   *   '<S2>/1-D Lookup Table4'
   */
  real_T pooled5[201];

  /* Expression: OCV_25
   * Referenced by: '<S2>/1-D Lookup Table1'
   */
  real_T uDLookupTable1_tableData[201];
} ConstP_SoCesti_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T current;                      /* '<Root>/current' */
  real_T voltage;                      /* '<Root>/voltage' */
} ExtU_SoCesti_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T SoC;                          /* '<Root>/SoC' */
} ExtY_SoCesti_T;

/* Real-time Model Data Structure */
struct tag_RTM_SoCesti_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T taskTime0;
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    time_T tFinal;
    boolean_T stopRequestedFlag;
  } Timing;
};

/* Block states (default storage) */
extern DW_SoCesti_T SoCesti_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_SoCesti_T SoCesti_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_SoCesti_T SoCesti_Y;

/* Constant parameters (default storage) */
extern const ConstP_SoCesti_T SoCesti_ConstP;

/* Model entry point functions */
extern void SoCesti_initialize(void);
extern void SoCesti_step(void);
extern void SoCesti_terminate(void);

/* Real-time Model object */
extern RT_MODEL_SoCesti_T *const SoCesti_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S22>/Reshape' : Reshape block reduction
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SoCesti'
 * '<S1>'   : 'SoCesti/EKF'
 * '<S2>'   : 'SoCesti/LU table'
 * '<S3>'   : 'SoCesti/RLS'
 * '<S4>'   : 'SoCesti/constant'
 * '<S5>'   : 'SoCesti/EKF/AHQR'
 * '<S6>'   : 'SoCesti/EKF/gain, co...'
 * '<S7>'   : 'SoCesti/EKF/m'
 * '<S8>'   : 'SoCesti/EKF/s.Eq'
 * '<S9>'   : 'SoCesti/EKF/AHQR/Ajacob'
 * '<S10>'  : 'SoCesti/EKF/AHQR/Hjacob'
 * '<S11>'  : 'SoCesti/EKF/AHQR/Subsystem'
 * '<S12>'  : 'SoCesti/EKF/gain, co.../K_k'
 * '<S13>'  : 'SoCesti/EKF/gain, co.../P'
 * '<S14>'  : 'SoCesti/EKF/gain, co.../P_p'
 * '<S15>'  : 'SoCesti/EKF/gain, co.../K_k/Transpose'
 * '<S16>'  : 'SoCesti/EKF/gain, co.../P_p/Transpose'
 * '<S17>'  : 'SoCesti/RLS/Subsystem'
 * '<S18>'  : 'SoCesti/RLS/Subsystem/MATLAB Function'
 * '<S19>'  : 'SoCesti/RLS/Subsystem/covariance'
 * '<S20>'  : 'SoCesti/RLS/Subsystem/gain'
 * '<S21>'  : 'SoCesti/RLS/Subsystem/reg. vector'
 * '<S22>'  : 'SoCesti/RLS/Subsystem/update'
 * '<S23>'  : 'SoCesti/RLS/Subsystem/y'
 * '<S24>'  : 'SoCesti/RLS/Subsystem/covariance/MATLAB Function'
 * '<S25>'  : 'SoCesti/RLS/Subsystem/covariance/Transpose'
 * '<S26>'  : 'SoCesti/RLS/Subsystem/gain/MATLAB Function'
 * '<S27>'  : 'SoCesti/RLS/Subsystem/gain/Transpose'
 * '<S28>'  : 'SoCesti/RLS/Subsystem/update/MATLAB Function'
 * '<S29>'  : 'SoCesti/RLS/Subsystem/update/Transpose'
 * '<S30>'  : 'SoCesti/RLS/Subsystem/y/Transpose'
 */
#endif                                 /* SoCesti_h_ */
