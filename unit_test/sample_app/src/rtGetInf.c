/*
 * File: rtGetInf.c
 *
 * Code generated for Simulink model 'unitTestModel'.
 *
 * Model version                  : 1.18
 * Simulink Coder version         : 9.0 (R2018b) 24-May-2018
 * C/C++ source code generated on : Thu Sep 27 22:02:27 2018
 *
 * Target selection: cfs_ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

/*
 * Abstract:
 *      Function to initialize non-finite, Inf
 */
#include "rtGetInf.h"
#define NumBitsPerChar 8U

/*
 * Initialize rtInf needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real_T rtGetInf(void)
{
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T inf         = 0.0;
    if (bitsPerReal == 32U)
    {
        inf = rtGetInfF();
    }
    else
    {
        union
        {
            LittleEndianIEEEDouble bitVal;
            real_T                 fltVal;
        } tmpVal;

        tmpVal.bitVal.words.wordH = 0x7FF00000U;
        tmpVal.bitVal.words.wordL = 0x00000000U;
        inf                       = tmpVal.fltVal;
    }

    return inf;
}

/*
 * Initialize rtInfF needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real32_T rtGetInfF(void)
{
    IEEESingle infF;
    infF.wordL.wordLuint = 0x7F800000U;
    return infF.wordL.wordLreal;
}

/*
 * Initialize rtMinusInf needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real_T rtGetMinusInf(void)
{
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T minf        = 0.0;
    if (bitsPerReal == 32U)
    {
        minf = rtGetMinusInfF();
    }
    else
    {
        union
        {
            LittleEndianIEEEDouble bitVal;
            real_T                 fltVal;
        } tmpVal;

        tmpVal.bitVal.words.wordH = 0xFFF00000U;
        tmpVal.bitVal.words.wordL = 0x00000000U;
        minf                      = tmpVal.fltVal;
    }

    return minf;
}

/*
 * Initialize rtMinusInfF needed by the generated code.
 * Inf is initialized as non-signaling. Assumes IEEE.
 */
real32_T rtGetMinusInfF(void)
{
    IEEESingle minfF;
    minfF.wordL.wordLuint = 0xFF800000U;
    return minfF.wordL.wordLreal;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
