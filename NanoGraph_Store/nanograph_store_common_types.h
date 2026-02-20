/* ----------------------------------------------------------------------
 * Project:      NanoGraph
 * Title:        NANOGRAPH_common.h
 * Description:  Common data definition
 *
 * $Date:        15 February 2023
 * $Revision:    V0.0.1
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2023 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */


#ifdef __cplusplus
 extern "C" {
#endif

#ifndef cNANOGRAPH_COMMON_TYPES_H
#define cNANOGRAPH_COMMON_TYPES_H


/* ------------------------------------------------------------------------------------------
    opaque access to the static area of the node
*/
typedef void* nanograph_handle_t;



/* ------------------------------------------------------------------------------------------
    stream buffers
*/
struct nanograph_xdmbuffer
{   intptr_t address;
    intptr_t size;
};

typedef struct nanograph_xdmbuffer nanograph_xdmbuffer_t;



/* ------------------------------------------------------------------------------------------
    stream services
*/
typedef void    (nanograph_services_t) (uint32_t service_command, intptr_t ptr1, intptr_t ptr2, intptr_t ptr3, intptr_t n);
typedef void (*p_nanograph_services_t) (uint32_t service_command, intptr_t ptr1, intptr_t ptr2, intptr_t ptr3, intptr_t n);


/* ------------------------------------------------------------------------------------------
    floating-point emulation
*/

//#ifdef NANOGRAPH_FLOAT_ALLOWED
#if 1
    #define fp32_t  float 
    #define fp64_t double 

#else

    #define fp32_t  struct float_t_struct 
    #define fp64_t  struct double_struct

    union floatb {
        uint32_t i;
        struct {
            unsigned m : 23;   /* Mantissa */
            unsigned e : 8;    /* Exponent */
            unsigned s : 1;    /* Sign bit */
        } b;
    };

    struct {
        unsigned m : 23;        /* Mantissa */
        unsigned e : 8;         /* Exponent */
        unsigned s : 1;         /* Sign bit */
    } float_t_struct;

    union doubleb {
        uint64_t i;
        struct {
            unsigned ml: 32;   /* Mantissa low */
            unsigned mh: 20;   /* Mantissa high */
            unsigned e : 11;   /* Exponent */
            unsigned s : 1;    /* Sign bit */
        } b;
    };

    struct {
        unsigned ml: 32;        /* Mantissa low */
        unsigned mh: 20;        /* Mantissa high */
        unsigned e : 11;        /* Exponent */
        unsigned s : 1;         /* Sign bit */
    } double_struct;

    #endif


/*----- ALL THE SERVICES ARE DISABLED BY DEFAULT   EXCEPT A SHORT LIST ALWAYS PRESENT  -----------*/
//                          index mask
// SERV_GROUP_INTERNAL    /* 0    1  internal : Semaphores, DMA, Clocks */
// SERV_GROUP_SCRIPT      /* 1    2  script : Node parameters  */
// SERV_GROUP_STDLIB      /* 2    4  stdlib,, string, malloc */
// SERV_GROUP_MATH        /* 3    8  math.h */
// SERV_GROUP_DSP_ML      /* 4   16  cmsis-dsp */
// SERV_GROUP_DEEPL       /* 5   32  cmsis-nn */



//    SERV_GROUP_INTERNAL                   /* 0  internal : Semaphores, DMA, Clocks */

//    SERV_GROUP_SCRIPT                     /* 1  script, set of node parameters  */
        
//    SERV_GROUP_STDLIB                     /* 2  stdlib,, string, malloc */
        
//    SERV_GROUP_MATH                       /* 3  math.h */
        
//    SERV_GROUP_DSP_ML                     /* 4  cmsis-dsp */

    //#define SERV_DSP_CASCADE_DF1_Q15     /* IIR filters, use SERV_CHECK_COPROCESSOR */
    //use #define PLATFORM_SERV_DSP_CASCADE_DF1_Q15 for specific implmentations
    typedef struct
    {       int16_t *pState;
      const int16_t *pCoeffs;
            int8_t numStages;
            int8_t postShift;
    }  generic_biquad_cascade_df1_inst_q15;

    typedef void    (df1_q15_init) (
        generic_biquad_cascade_df1_inst_q15 * S,
            uint8_t numStages,
      const int16_t * pCoeffs,
            int16_t * pState,
            int8_t postShift );

    typedef void    (df1_q15) (         /* platform-accelerated DF1 will use this template */
      const generic_biquad_cascade_df1_inst_q15 * S,
      const int16_t * pSrc,
            int16_t * pDst,
            uint32_t blockSize );

    //#define SERV_DSP_CASCADE_DF1_F32
    //use #define PLATFORM_SERV_DSP_CASCADE_DF1_F32 for specific implmentations
    typedef struct
    {       uint32_t numStages;
            float *pState;
      const float *pCoeffs;
    } generic_biquad_cascade_df1_inst_f32;

    typedef void (df1_init_f32) (
        generic_biquad_cascade_df1_inst_f32 * S,
          uint8_t numStages,
    const float * pCoeffs,
          float * pState);

    typedef void (df1_f32) (        /* platform-accelerated DF1 will use this template */
    const generic_biquad_cascade_df1_inst_f32 * S,
    const float * pSrc,
          float * pDst,
          uint32_t blockSize);


    //#define SERV_DSP_WINDOW                
    //#define SERV_DSP_WINDOW_DB             
    //#define SERV_DSP_rFFT_Q15           /* RFFT windowing, module, dB , use SERV_CHECK_COPROCESSOR */
    //#define SERV_DSP_rFFT_F32           /* default FFT with tables rebuilded */
    //#define SERV_DSP_cFFT_Q15           /* cFFT windowing, module, dB */
    //#define SERV_DSP_cFFT_F32           

// SERV_GROUP_DEEPL              /* 5  cmsis-nn */
    //#define SERV_ML_FC                  /* fully connected layer Mat x Vec */
    //#define SERV_ML_CNN                 /* convolutional NN : 3x3 5x5 fixed-weights */



/*---------------------------------------------------------------------------------------------------------------------*/
#endif /* cNANOGRAPH_COMMON_TYPES_H */
/*
 * -----------------------------------------------------------------------
 */
#ifdef __cplusplus
}
#endif
    

