/* ----------------------------------------------------------------------
 * Project:      NanoGraph
 * Title:        nanograph_common.h
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
 */\


#ifdef __cplusplus
 extern "C" {
#endif

#ifndef cNANOGRAPH_COMMON_CONST_H
#define cNANOGRAPH_COMMON_CONST_H


/* ----------------------------------------------------------------------------------------------------------------
    NANOGRAPH NODES 
*/

#define MAX_NB_NODE_ENTRY_POINTS 32

#define NODE_ALL_PARAM 0u               /* set all the paramters */

#define NODE_TASKS_COMPLETED 0u         /* execution completed */
#define NODE_TASKS_NOT_COMPLETED 1u

#define MAX_NB_NANOGRAPH_PER_NODE 8        /* I/O streams per node, see graph "NBARCW_LW0" */





#define U(x) ((uint32_t)(x)) /* for MISRA-2012 compliance to Rule 10.4 */


/* 
   ================================= nanograph_format  FORMATS =======================================
    
    Format 23+4_offsets for buffer BASE ADDRESS
    Frame SIZE and ring indexes are using 22bits linear (0..4MB)


        Word0: Frame size, interleaving scheme, arithmetics raw data type
        Word1: time-stamp, domain, nchan, physical unit (pixel format, IMU interleaving..)
        Word2: Sampling-rate or specific to IO domain
        Word3: specific to IO Domain

*/

#define NANOGRAPH_FORMAT_SIZE_W32 4u     /*  digital, common part of the format  */
/*
*   _FMT0 word 0 : common to all domains : frame size
*   _FMT1 word 1 : common to all domains : time-stamp, nchan, raw format, interleaving, domain, size extension
*   _FMT2 word 2 : specific to domains : sampling rate
*   _FMT3 word 3 : specific to domains : hashing, channel mapping  ..
*/

/* for MISRA-2012 compliance to Rule 10.4 */
#define U8(x) ((uint8_t)(x)) 
#define S8(x) ((int8_t)(x)) 

//enum time_stamp_format_type {
////+enum frame_format_synchro {
#define SYNCHRONOUS 0u              /* tells the output buffer size is NOT changing */
#define ASYNCHRONOUS 1u             /* tells the output frame length is variable, input value "Size" tells the maximum value  
//                                       data format : optional time-stamp (nanograph_time_stamp_format_type)
//                                                    domain [2bits] and sub-domain [6bits rfc8428]
//                                                    payload : [nb samples] [samples]  */

// NANOGRAPH_TIME32D  ssssssssssssssssqqqqqqqqqqqqqqqq q17.15 [s] (36h, +/- 30us) time difference */   
                /* |123456789|123456789|123456789|123456789|123456789|123456789|123 */
// NANOGRAPH_TIME64   0000ssssssssssssssssssssssssssssssssqqqqqqqqqqqqqqqqqqqqqqqqqqqq q32.28 [s] 140 Y +Q28 [s] */   
#define NO_TIMESTAMP    0u
#define FRAME_COUNTER   1u          /* 32bits counter of data frames */
#define TIMESTAMP_ABS   2u          /*  absolute frame distance in [s] from Unix Epoch */
#define TIMESTAMP_REL   3u          /*  relative frame distance in [s] */


//enum hashing_type {
#define NO_HASHING 0u                /* cipher protocol under definition */
#define HASHING_ON 1u                /* the stream is ciphered (HMAC-SHA256 / nanograph_encipher XTEA)*/

//enum frame_format_type {
#define FMT_INTERLEAVED 0u          /* "arc_descriptor_interleaved" for example L/R audio or IMU stream..   */
#define FMT_DEINTERLEAVED_1PTR 1u   /* pointer to the first channel, next base address is frame size/nchan */
#define FMT_DEINTERLEAVED_UNPACK 2u /* audioreach de-interleaved unpack : LLLL__ RRRR__ using two buffers */

//enum direction_rxtx {
#define IODIRECTION_RX 0u          /* RX from the Graph pont of view */
#define IODIRECTION_TX 1u


/*  ---------------------------------
    SIZE_EXT_FMT0 / COLLISION_ARC / NODE memory bank

    -8 bits-[--------24 bits ------]
    1_987654321_987654321_987654321_
    cccccccc________________________  collision byte in the WRITE word32, IO affinity ARCHID_LW0
    R_______________________________  flag: relocatable memory segment
    _X______________________________  Flag telling the graph section is accessed "in-place" (COPY_IN_RAM_FMT0)
    ___<-5->________________________  long offset of the buffer base address: virtual MMU to platform "VID"
    ________EXT_____________________  extension 3bits shifter
    ___________s44443333222211110000  5 hex signed digits[1+20b] = FrameSize and Arc-R/W
            <--SIZE_EXT_FMT0_MSB--->  R/W arc index (with EXT) leaves one byte
       <----SIZE_EXT_OFF_FMT0_MSB-->  base addresses on 29bits 
    <->                               tells the loader to copy a graph section in RAM
    
   max = +/- 0x000FFFFF << (EXT x 2) 

   There are 32 OFFsets of 64bits, 
   EXT  Shift   Granule range(+/- 20bits)
   0    0       1       +/- 1MB     
   1    2       4       +/- 4MB     
   2    4       16      +/- 16MB
   3    6       64      +/- 64MB
   4    8       256     +/- 256MB
   5    10      1K      +/- 1GB
   6    14      16K     +/- 16GB    (EXT6)
   7    18      256K    +/- 256GB   (EXT7)

   max = (+/-)0x000FFFFF << (2xEXT) = +/-1M x (2<<EXT) for EXT=[0..5] (EXT6<<14, EXT7<<18)
   ----------------------------------- 
*/
    #define SIZE_EXT_OFF_FMT0_MSB 28u /*       29 = offsets(5) + EXT(3) + sign(1) + size(20) */
    #define  ADDR_OFFSET_FMT0_MSB 28u
    #define  ADDR_OFFSET_FMT0_LSB 24u /*  5    offsets */
    #define     SIZE_EXT_FMT0_MSB 23u /*       24 = EXT(3) + sign(1) + size(20) */
    #define    EXTENSION_FMT0_MSB 23u /*    */
    #define    EXTENSION_FMT0_LSB 21u /*  3 */
    #define  SIGNED_SIZE_FMT0_MSB 20u /*       21 bits for sign(1) + size(20) */
    #define    SIZE_SIGN_FMT0_MSB 20u /*    */
    #define    SIZE_SIGN_FMT0_LSB 20u /*  1 */
    #define         SIZE_FMT0_MSB 19u /*    */
    #define         SIZE_FMT0_LSB  0u /* 20 */
    #define  SIGNED_SIZE_FMT0_LSB  0u /*    */
    
    #define     SIZE_EXT_FMT0_LSB  0u /*    */
    #define SIZE_EXT_OFF_FMT0_LSB  0u /*    */
    
    /* number of physical memory banks of the processor, for the graph processing */
    #define MAX_PROC_MEMBANK ((1<< (ADDR_OFFSET_FMT0_MSB - ADDR_OFFSET_FMT0_LSB +1))-1)
    



/*  WORD 0 - frame size --------------- */
#define FRAMESZ_FMT0   0u          
    /* A "frame" is the combination of several channels sampled at the same time            */
    /* frame size in bytes, data interleaved or not                                         */
    /* A value =0 means the size is any or defined by the IO AL.                            */
    /* in node manifests it gives the minimum input size (grain) before activating the node */
    /* For sensors delivering burst of data not isochronous, it gives the maximum           */
    /* framesize; same comment for the sampling rate.                                       */
    /* The frameSize is including the time-stamp field                                      */

    #define unused____FMT0_MSB  31u /*  8 reserved */
    #define unused____FMT0_LSB  24u 
    #define FRAMESIZE_FMT0_MSB  SIZE_EXT_FMT0_MSB
    #define FRAMESIZE_FMT0_LSB  SIZE_EXT_FMT0_LSB

/*- WORD 1 - domain, sub-types , size extension, time-stamp, raw format, interleaving, nchan  -------------*/
#define NCHANDOMAIN_FMT1   1
    #define unused____FMT1_MSB  31u /*     */  
    #define unused____FMT1_LSB  29u /* 3 reserved */
    #define   SUBTYPE_FMT1_MSB  28u /*     */  
    #define   SUBTYPE_FMT1_LSB  22u /* 7  sub-type for pixels and analog formats (NANOGRAPH_SUBT_ANA_)  */
    #define    DOMAIN_FMT1_MSB  21u 
    #define    DOMAIN_FMT1_LSB  18u /* 4  NANOGRAPH_IO_DOMAIN = DOMAIN_FMT1 */
    #define       RAW_FMT1_MSB  17u
    #define       RAW_FMT1_LSB  12u /* 6  arithmetics nanograph_raw_data 6bits (0..63)  */
    #define  TSTPSIZE_FMT1_MSB  11u 
    #define  TSTPSIZE_FMT1_LSB  10u /* 2  16/32/64/64TXT time-stamp time format */
    #define  TIMSTAMP_FMT1_MSB   9u 
    #define  TIMSTAMP_FMT1_LSB   7u /* 3  time_stamp_format_type for time-stamped streams for each interleaved frame */
    #define INTERLEAV_FMT1_MSB   6u       
    #define INTERLEAV_FMT1_LSB   5u /* 2  interleaving : frame_format_type */
    #define   NCHANM1_FMT1_MSB   4u 
    #define   NCHANM1_FMT1_LSB   0u /* 5  nb channels-1 [1..32] */

/*  WORD2 - sampling rate */
#define SAMPLINGRATE_FMT2   2
    /*--------------- WORD 2 -------------*/
    #define      FS1D_FMT2_MSB  31u /* 32 IEEE-754, 0 means "asynchronous" or "any" */
    #define      FS1D_FMT2_LSB   0u /*    [Hz] */

/*  WORD 3 for IO_DOMAIN_AUDIO_IN and IO_DOMAIN_AUDIO_OUT */
#define DOMAINSPECIFIC_FMT3   3
    #define unused____FMT3_MSB U(31) /* 8b   */
    #define unused____FMT3_LSB U(24) 
    #define AUDIO_MAP_FMT3_MSB U(23) /* 24 mapping of channels example of 7.1 format (8 channels): */
    #define AUDIO_MAP_FMT3_LSB U( 0) /*     FrontLeft, FrontRight, FrontCenter, LowFrequency, BackLeft, BackRight, SideLeft, SideRight ..*/

    
    /*--------------- WORD 3 for IO_DOMAIN_MOTION */
    //enum imu_channel_format /* uint8_t : data interleaving possible combinations */
    typedef enum  
    {   aXg0m0=1, /* only accelerometer */
        a0gXm0=2, /* only gyroscope */
        a0g0mX=3, /* only magnetometer */
        aXgXm0=4, /* A + G */
        aXg0mX=5, /* A + M */
        a0gXmX=6, /* G + M */
        aXgXmX=7, /* A + G + M */
    } imu_channel_format;
    #define MOTION_MAPPING_FMT3_MSB U(31) /* 28 reserved for mapping information */
    #define MOTION_MAPPING_FMT3_LSB U( 4) 
    #define MOTION_TEMPERA_FMT3_MSB U( 3) /*  1 temperature capture */
    #define MOTION_TEMPERA_FMT3_LSB U( 3) /*     */
    #define MOTION_DATAFMT_FMT3_MSB U( 2) /*  3 imu_channel_format : A, A+G, A+G+M, .. MSB used for the temperature */
    #define MOTION_DATAFMT_FMT3_LSB U( 0) /*     */
   

    /*--------------- WORD 3  for IO_DOMAIN_2D_IN and IO_DOMAIN_2D_OUT */
    // IO_DOMAIN_2D_IN              camera sensor, video sensor */
    // IO_DOMAIN_2D_OUT             display, led matrix, */

    /* EXTEND_FMT1 gives some margin of size extension */
    typedef enum 
    { 
        R2D_TBD=0, R2D_1_1=1, R2D_4_3=2, R2D_16_9=3, R2D_3_2=4 
    }   ratio_2d_fmt3 ;

    #define   unused_____FMT3_MSB U(31) /* 14 */
    #define   unused_____FMT3_LSB U(18)
    #define   I2D_BORDER_FMT3_MSB U(17) /*  2 pixel border 0,1,2,3   */
    #define   I2D_BORDER_FMT3_LSB U(16)
    #define I2D_VERTICAL_FMT3_MSB U(15) /*  1 set to 0 for horizontal, 1 for vertical */
    #define I2D_VERTICAL_FMT3_LSB U(15)
    #define     RATIO_2D_FMT2_MSB U(14) /*  3 ratio_2d_fmt3 */
    #define     RATIO_2D_FMT2_LSB U(12) 
    #define I2D_SMALLDIM_FMT3_MSB U(11) /* 12 smallest pixel dimension, the largest comes with a division with */
    #define I2D_SMALLDIM_FMT3_LSB U( 0) /*    largest dimension = (frame_size - time_stamp_size)/smallest_dimension 
                                              or  largest dimension = ratio x smallest_dimension  */
//
//
//    /*  WORD 4 IO domain specific */
//#define DOMAINSPECIFIC_FMT4   4
//    #define unused____FMT4_MSB U(31) 
//    #define unused____FMT4_LSB U( 0) 
//
//
//    /*  WORD 5 IO domain specific */
//#define DOMAINSPECIFIC_FMT5   5
//    #define unused____FMT5_MSB U(31) 
//    #define unused____FMT5_LSB U( 0) 

/*====================================================================================================================*/                          
/*
    commands 
        from the application to the graph scheduler         arm_graph_interpreter(command,  arm_nanograph_instance_t *S, uint8_t *, uint32_t);
        from the graph scheduler to the nanoApps            S->address_node (command, nanograph_handle_t instance, nanograph_xdmbuffer_t *, uint32_t *);
        from the Scripts to the IO configuration setting    void arm_nanograph_services (uint32_t command, intptr_t ptr1, intptr_t ptr2, intptr_t ptr3, intptr_t n) 
*/

    /*  FROM APP TO SCHEDULER :     arm_graph_interpreter (NANOGRAPH_RESET, &nanograph_instance, 0, 0); 
        FROM SCHEDULER to NODE :     devxx_fyyyy (NANOGRAPH_RESET, &node_instance, &memreq, &status); 
            Command + nb arcs, preset 0..15, TAG 0..255
        -  (NANOGRAPH_RESET, ptr1, ptr2, ptr3); 
            ptr1 = instance pointer, memory banks
            ptr2 = nanograph_services function address, followed by all the arc format
        -  (NANOGRAPH_SET_PARAMETER, ptr1, ptr2, ptr3); 
            ptr1 = instance
            ptr2 = byte pointer to parameters, depends on the TAG 
        -  (NANOGRAPH_READ_PARAMETER, ptr1, ptr2, ptr3); 
            ptr1 = instance
            ptr2 = parameter data destination, depends on the TAG
        -  (NANOGRAPH_RUN, ptr1, ptr2, ptr3); 
            ptr1 = instance
            ptr2 = list of XDM arc buffers (X,n) , the size field means :
                 rx arc . size = amount of data available for processing
                 tx arc . size = amount of free area in the buffer 
                NODE updates the XDM size fields with :
                 rx arc . size = amount of data consumed
                 tx arc . size = amount of data produced
        -  (NANOGRAPH_STOP, ptr1, ptr2, ptr3); 
    */
    #define NANOGRAPH_RESET            1u  /* arm_graph_interpreter(NANOGRAPH_RESET, *instance, * memory_results) */
        #define COMMDEXT_COLD_BOOT    0u  /* if (NANOGRAPH_COLD_BOOT == RD(command, COMMDEXT_CMD)) */
        #define COMMDEXT_WARM_BOOT    1u  /* Reset + restore memory banks from retention */
        #define COMMDEXT_DYN_MALLOC   2u  /* pre-reset phase : ask for the amount of memory to allocate */

    #define NANOGRAPH_SET_PARAMETER    2u  /* APP sets NODE parameters node instances are protected by multithread effects when 
                                          changing parmeters on the fly, used to exchange the unlock key */
            



    //#define NANOGRAPH_SET_IO_CONFIG NANOGRAPH_SET_PARAMETER 
    /* @@@ TODO
       reconfigure the IO : p_io_function_ctrl(  + (FIFO_ID<<NODE_TAG_CMD), 0, new_configuration_index) 
       
       io_power_mode 0 ; to set the device at boot time in stop / off (0)
                       ; running mode(1) : digital conversion (BIAS always powered for analog peripherals)
                       ; running mode(2) : digital conversion BIAS shut-down between conversions
                       ; Sleep (3) Bias still powered but not digital conversions            
    */

    #define NANOGRAPH_READ_PARAMETER   3u   /* used from script */
    #define NANOGRAPH_RUN              4u   /* arm_graph_interpreter(NANOGRAPH_RUN, instance, *in_out) */
    #define NANOGRAPH_STOP             5u   /* arm_graph_interpreter(NANOGRAPH_STOP, instance, 0)  node calls free() if it used stdlib's malloc */
    #define NANOGRAPH_UPDATE_RELOCATABLE 6u /* update the nanoAppRT pointers to relocatable memory segments */

    #define NANOGRAPH_SET_BUFFER       7u   /* platform_IO(NANOGRAPH_SET_BUFFER, *data, size)  */
    #define NANOGRAPH_READ_DATA        8u   /* COMMAND_SSRV syscall read access to arc data */
    #define NANOGRAPH_WRITE_DATA       9u   /* COMMAND_SSRV syscall write access to arc data */

    #define NANOGRAPH_LIBRARY          10u  /* other functions of the node (IIR parameters compute, ..) */

    #define NOWAIT_OPTION_SSRV      0u   /* OPTION_SSRV  stall or not the COMMAND */
    #define   WAIT_OPTION_SSRV      1u


/*  FROM THE GRAPH SCHEDULER TO THE NANOAPPS   NODE_COMMANDS  */
    #define  POSITION_CMD_MSB U(31)       
    #define      NARC_CMD_MSB U(23)       
    #define      NARC_CMD_LSB U(20) /* 4 number of arcs */
    #define    PRESET_CMD_MSB U(19)       
    #define    PRESET_CMD_LSB U(16) /* 4  #16 presets */
    #define  POSITION_CMD_LSB U(16) /* 16 node position (script calls set_param to a node at this position) */

    #define  NODE_TAG_CMD_MSB U(15) /*    parameter, function selection / debug arc index / .. */      
    #define  NODE_TAG_CMD_LSB U( 8) /* 8  instanceID for the trace / FIFO_ID for status checks */
    #define   UNUSED1_CMD_MSB U( 7)       
    #define   UNUSED1_CMD_LSB U( 6) /* 2 _______ */
    #define  COMMDEXT_CMD_MSB U( 5)       
    #define  COMMDEXT_CMD_LSB U( 4) /* 2 command option (MALLOC, RESET + warmboot, (SET_PARAM + wait)  */
    #define   COMMAND_CMD_MSB U( 3)       
    #define   COMMAND_CMD_LSB U( 0) /* 4 command */

    #define PACK_COMMAND(SWCTAG,PRESET,NARC,EXT,CMD) (((SWCTAG)<<NODE_TAG_CMD_LSB)|((PRESET)<<PRESET_CMD_LSB)|((NARC)<<NARC_CMD_LSB)|((EXT)<<COMMDEXT_CMD_LSB)|(CMD))


/*=============================================================================================*/
/*
    "SERV_COMMAND"  from the nodes, to "NanoGraph_services"

    void NanoGraph_services (uint32_t command, uint8_t *ptr1, uint8_t *ptr2, uint8_t *ptr3, uint32_t n)

    commands from the NODE to Stream
    16 family of commands:
    - 1 : internal to NanoGraph, reset, debug trace, report errors , described here
            - Un/Lock a section of the graph
            - Jump to +/- N nodes in the list, switch(UC_1) case u1:list of nodes; case u2: list2 ..;
                script resets the activated nodes if overlaid with other ones
            - system regsters access: who am I ?
            - time measurement for cycle benchmarks (read timer /time difference)
    - 2 : arc access for SCRIPT : pointer, last data, debug fields, format changes
    - 3 : format converters (time, raw data)
    - 4 : stdlib.h subset (time, stdio)
    - 5 : math.h subset
    - 6 : Interface to CMSIS-DSP
    - 7 : Interface to CMSIS-NN
    - 8 : Multimedia audio library
    - 9 : Image processing library
    - 10..15 : reserved

    each family can define 256 operations (TAG_CMD_LSB)
*/
/* ============================================================================================ */

#define NOCOMMAND_SSRV 0u
#define NOOPTION_SSRV 0u
#define NOTAG_SSRV 0u

/* -------------------------------------------------------
    NanoGraph_services                  CottfFFg 
            
            Command                     C
            Option                       o
            TAG                           tt
            sub Function                    f
            Function                         FF
            Service Group                      g
   -------------------------------------------------------
*/
#define  COMMAND_SSRV_MSB U(31)       
#define  COMMAND_SSRV_LSB U(28) /* 4   C   set/init/run w/wo wait completion, in case of coprocessor usage */
#define   OPTION_SSRV_MSB U(27)       
#define   OPTION_SSRV_LSB U(24) /* 4   o   compute accuracy, in-place processing, frame size .. */
#define      TAG_SSRV_MSB U(23)       
#define      TAG_SSRV_LSB U(16) /* 8   tt  parameter of the function  */
#define FUNCTION_SSRV_MSB U(15)       
#define SUBFUNCT_SSRV_MSB U(15)       
#define SUBFUNCT_SSRV_LSB U(12) /* 4   f   16 sub functions or parameters on MSB */
#define FUNCTION_SSRV_LSB U( 4) /* 12  fFF functions/group x 16 subfunct, total = 4K */
#define    GROUP_SSRV_MSB U( 3)       
#define    GROUP_SSRV_LSB U( 0) /* 4   g   16 service groups */


#define PACK_SERVICE(COMMAND,OPTION,TAG,FUNC,GROUP) \
     ((COMMAND)<<COMMAND_SSRV_LSB)  | \
   ( ((OPTION) <<OPTION_SSRV_LSB)   | \
   ( ((TAG)    <<TAG_SSRV_LSB)      | \
   ( ((FUNC)   <<FUNCTION_SSRV_LSB) | \
     ((GROUP)  <<GROUP_SSRV_LSB)     )))

/* mask for node_mask_library, is there a need for a registered return address (Y/N)  */
#define SERV_GROUP_INTERNAL     0u  /* 1   N internal : Semaphores, DMA, Clocks */
#define SERV_GROUP_SCRIPT       1u  /* 2   N script : Node parameters  */
#define SERV_GROUP_STDLIB       2u  /* 4   Y Compute : malloc, string */
#define SERV_GROUP_MATH         3u  /* 8   N math.h */
#define SERV_GROUP_DSP_ML       4u  /* 16  N cmsis-dsp */
#define SERV_GROUP_DEEPL        5u  /* 32  N cmsis-nn */
#define SERV_GROUP_MM_AUDIO     6u  /* 64  Y speech/audio processing */
#define SERV_GROUP_MM_IMAGE     7u  /* 128 Y image processing */


/* --------------------------------------------------------------------------- */
/* GROUP_SSRV = 1/SERV_GROUP_INTERNAL ---------------------------------------- */
/* --------------------------------------------------------------------------- */

    /*  FUNCTION_SSRV GROUP : ERRORS ------------------------------- */
#define PLATFORM_ERROR                  0x901   /* error to report to the application */

#define SERV_INTERNAL_SLEEP_CONTROL                          0
#define SERV_INTERNAL_CPU_CLOCK_UPDATE                       1
#define SERV_INTERNAL_READ_MEMORY                            2
#define SERV_INTERNAL_READ_MEMORY_FAST_MEM_ADDRESS           3
#define SERV_INTERNAL_SERIAL_COMMUNICATION                   4
#define SERV_INTERNAL_MUTUAL_EXCLUSION_WR_BYTE_AND_CHECK_MP  5
#define SERV_INTERNAL_MUTUAL_EXCLUSION_RD_BYTE_MP            6
#define SERV_INTERNAL_MUTUAL_EXCLUSION_WR_BYTE_MP            7
#define SERV_INTERNAL_MUTUAL_EXCLUSION_CLEAR_BIT_MP          8
#define SERV_INTERNAL_INTERNAL_READ_TIME                     9
#define SERV_INTERNAL_READ_TIME64                           10
#define SERV_INTERNAL_READ_TIME32                           11
#define SERV_INTERNAL_READ_TIME16                           12
#define SERV_INTERNAL_KEYEXCHANGE                           13


/* --------------------------------------------------------------------------- */
/* GROUP_SSRV = 2/SERV_SCRIPT ------------------------------------------------ */
/* --------------------------------------------------------------------------- */

/*     List of                      COMMAND_SSRV  (4bits) */
#define SERV_SCRIPT_RESET               0x1
#define SERV_SCRIPT_NODE                0x2
#define SERV_SCRIPT_SCRIPT              0x3  /* node control from scripts */
                                       
#define SERV_SCRIPT_FORMAT_UPDATE       0x4  /* change stream format from NODE media decoder, script applying change of 
                                                use-case (IO_format, vocoder frame-size..): sampling, nb of channel, 2D frame size */
    /* SYSCALL_NODE (CMD=set/read_param + TAG, Node offset, Pointer, Nbytes) */
    #define SYSCALL_FUNCTION_SSRV_NODE      1u       

/*      #define NANOGRAPH_RESET                1
        #define NANOGRAPH_SET_PARAMETER        2
        #define NANOGRAPH_READ_PARAMETER       3
        #define NANOGRAPH_RUN                  4
        #define NANOGRAPH_STOP                 5
        #define NANOGRAPH_UPDATE_RELOCATABLE   6
        #define NANOGRAPH_SET_BUFFER           7
        #define NANOGRAPH_READ_DATA            8
        #define NANOGRAPH_WRITE_DATA           9 */

/*#define SERV_SCRIPT_FORMAT_UPDATE_FS 3u         NODE information for a change of stream format, sampling, nb of channel */
/*#define SERV_SCRIPT_FORMAT_UPDATE_NCHAN 4u      raw data sample, mapping of channels, (web radio use-case) */
/*#define SERV_SCRIPT_FORMAT_UPDATE_RAW 5u          */


#define SERV_SCRIPT_SECURE_ADDRESS      0x6   /* this call is made from the secured address */
#define SERV_SCRIPT_AUDIO_ERROR         0x7   /* PLC applied, Bad frame (no header, no synchro, bad data format), bad parameter */
#define SERV_SCRIPT_DEBUG_TRACE         0x8   /* 1b, 1B, 16char */
#define SERV_SCRIPT_DEBUG_TRACE_STAMP   0x9   
#define SERV_SCRIPT_AVAILABLE           0xA   
#define SERV_SCRIPT_SETARCDESC          0xB   /* buffers holding MP3 songs.. rewind from script, 
                                                   switch a NN model to another, change a parameter-set using arcs */


//SERV_SCRIPT_DEBUG_TRACE, SERV_SCRIPT_DEBUG_TRACE_1B, SERV_SCRIPT_DEBUG_TRACE_DIGIT, 
// 
//SERV_SCRIPT_DEBUG_TRACE_STAMPS, SERV_SCRIPT_DEBUG_TRACE_STRING,
// 
//NANOGRAPH_SAVE_HOT_PARAMETER, 

//NANOGRAPH_LOW_POWER,     /* interface to low-power platform settings, "wake-me in 24h with deep-sleep in-between" */
//                          " I have nothing to do most probably for the next 100ms, do what is necessary "

//NANOGRAPH_PROC_ARCH,     /* returns the processor architecture details, used before executing specific assembly codes */



    /* SYSCALL_ARC (CMD read/set_param_descriptor, read/write_data, arc ID, Pointer, Nbytes) 
        descriptor = filling/empty state, locked flag, R/W index, buffer size
            debug information, time-stamp of the last access
            format update ? FS, frame-size ?
        read_data option : without updating the read pointer

        script language : SYSCALL ARC r-CMD r-ARC r-ADDR r-N
        (*al_func)(
            PACK_SERVICE(NANOGRAPH_READ_DATA, NOWAIT_OPTION_SSRV, PARAM_TAG, 
                SYSCALL_FUNCTION_SSRV_ARC, SERV_GROUP_SCRIPT), 
            arcID
            address of the data to be read,
            unused
            number of bytes
            );

    */
    #define SYSCALL_FUNCTION_SSRV_ARC       2u

    /* SYSCALL_CALLBACK (CMD , r1, r2, r3, r4) 
            depends on the application. Address the case of Arduino libraries
            example specific : IP address, password to share, Ping IP to blink the LED, read RSSI, read IP@
    */
    #define SYSCALL_FUNCTION_SSRV_CALLBACK  3u

    /* SYSCALL_IO       CMD(=set/read_param) FWIOIDX_IOFMT0 to select another A/D or GPIO (when the graph is in RAM)
                        CMD(=read/write_data) domain-specific settings
                        CMD(=stop/reset/run) stop and initiate data transfer
    */
    #define SYSCALL_FUNCTION_SSRV_IO        4u

    /* SYSCALL_DEBUG    receive remote commands, send string of debug data
    */
    #define SYSCALL_FUNCTION_SSRV_DEBUG     5u

    /* SYSCALL_COMPUTE  call compute library
    *                   Compute Median/absMax from data in a circular buffer of the Heap + mean/STD

        Math library: Commonly used single-precision floating-point functions:
            Basic operations: ceilf, fabsf, floorf, fmaxf, fminf, fmodf, roundf, lroundf, remainderf
            Exponential/power functions: expf, log2f, powf, sqrtf
            Trigonometric/hyperbolic functions: sinf, cosf, tanf, asinf, acosf, atan2f, tanhf 
    */
    #define SYSCALL_FUNCTION_SSRV_COMPUTE   6u
                                          
    /* SYSCALL_TIME_IDLE    Timer setting, read the time in different formats (16/32/64/delta)
                        default idle mode commands
                        compute time elapsed from today, from a reference, from reset, UTC/local time
    */
    #define SYSCALL_FUNCTION_SSRV_TIME_IDLE 7u

    /* SYSCALL_LOWLEVEL     TBD : low-level I2C/peripheral/memory access
    *                   Call a relocatable native-binary section in the Param area
    */
    #define SYSCALL_FUNCTION_SSRV_LOWLEVEL  8u

    // for scripts/Nodes: fast data moves
    #define SERV_SCRIPT_DMA_SET             9u      /* set src/dst/length */
    #define SERV_SCRIPT_DMA_START          10u
    #define SERV_SCRIPT_DMA_STOP           11u
    #define SERV_SCRIPT_DMA_CHECK          12u

/* --------------------------------------------------------------------------- */
/* GROUP_SSRV = 3/SERV_CONVERSION -------------------------------------------- */
/* --------------------------------------------------------------------------- */
    #define SERV_CONVERSION_INT16_FP32 1


/* --------------------------------------------------------------------------- */
/* GROUP_SSRV = 4/SERV_STDLIB ------------------------------------------------ */
/* --------------------------------------------------------------------------- */

    /* stdlib.h */
    /* string.h */
    //NANOGRAPH_MEMSET, NANOGRAPH_STRCHR, NANOGRAPH_STRLEN,
    //NANOGRAPH_STRNCAT, NANOGRAPH_STRNCMP, NANOGRAPH_STRNCPY, NANOGRAPH_STRSTR, NANOGRAPH_STRTOK,
    #define NANOGRAPH_ATOF      3u
    #define NANOGRAPH_ATOI      4u
    #define NANOGRAPH_MEMSET    5u
    #define NANOGRAPH_STRCHR    6u
    #define NANOGRAPH_STRLEN    7u
    #define NANOGRAPH_STRNCAT   8u
    #define NANOGRAPH_STRNCMP   9u
    #define NANOGRAPH_STRNCPY  10u
    #define NANOGRAPH_STRSTR   11u
    #define NANOGRAPH_STRTOK   12u
    //NANOGRAPH_ATOF, NANOGRAPH_ATOI
    #define NANOGRAPH_FREE     13u
    #define NANOGRAPH_MALLOC   14u


/* --------------------------------------------------------------------------- */
/* GROUP_SSRV = 5/SERV_MATH -------------------------------------------------- */
/* --------------------------------------------------------------------------- */

    /* minimum service : tables of 64 data RAND, SRAND */
    #define NANOGRAPH_RAND     1 /* (NANOGRAPH_RAND + OPTION_SSRV(seed), *ptr1, 0, 0, n) */
    #define NANOGRAPH_SRAND    2
    #define SERV_TABLE_SIN      
    #define SERV_TABLE_TAN      
    #define SERV_TABLE_ATAN      
    #define SERV_TABLE_SQRT      
    #define SERV_TABLE_LOG


    /* returns a code corresponding to the processor architecture and its FPU options */
#define SERV_CHECK_ARCHITECTURE 2
    /* time.h */
    //NANOGRAPH_ASCTIMECLOCK, NANOGRAPH_DIFFTIME, NANOGRAPH_SYS_CLOCK (ms since reset), NANOGRAPH_TIME (linux seconds)
    //NANOGRAPH_READ_TIME (high-resolution timer), NANOGRAPH_READ_TIME_FROM_START, 
    //NANOGRAPH_TIME_DIFFERENCE, NANOGRAPH_TIME_CONVERSION,  
    // 
    //NANOGRAPH_TEAM

    /* From Android CHRE  https://source.android.com/docs/core/interaction/contexthub
    String/array utilities: memcmp, memcpy, memmove, memset, strlen
    Math library: Commonly used single-precision floating-point functions:
    Basic operations: ceilf, fabsf, floorf, fmaxf, fminf, fmodf, roundf, lroundf, remainderf
    Exponential/power functions: expf, log2f, powf, sqrtf
    Trigonometric/hyperbolic functions: sinf, cosf, tanf, asinf, acosf, atan2f, tanhf
    */
    #define SERV_MATH_SQRT_Q15       15
    #define SERV_MATH_SQRT_F32       16
    #define SERV_MATH_LOG_Q15        17
    #define SERV_MATH_LOG_F32        18

    #define SERV_MATH_SINE_Q15       19
    #define SERV_MATH_SINE_F32       20
    #define SERV_MATH_COS_Q15        21
    #define SERV_MATH_COS_F32        22
    #define SERV_MATH_ATAN2_Q15      23
    #define SERV_MATH_ATAN2_F32      24
               
    #define SERV_MATH_SORT           3 


/* --------------------------------------------------------------------------- */
/* GROUP_SSRV = 6/SERV_DSP_ML ------------------------------------------------ */
/* --------------------------------------------------------------------------- */

    /* list from ETAS "Embedded AI Coder" :
    * Batchnorm, Convolutions, Depthwise Convolutions, LSTM, Fully Connected, Elementwise Add, Sub, Mul, 
    Softmax, Relu, Leaky Relu, Logistic, Padding, StridedSlice, Tanh, MaxPooling, AveragePooling and 
    TransposeConv. It supports the data types int8 and float32.
    */
    /* minimum service : IIRQ15/FP32, DFTQ15/FP32 */
            /* FUNCTION_SSRV */
    #define SERV_DSP_CHECK_COPROCESSOR  1u   /* check for services() */
    #define SERV_DSP_CHECK_END_COMP     2u   /* check completion for the caller */
    #define SERV_DSP_DFT_Q15            9u   /* DFT/Goertzel windowing, module, dB */
    #define SERV_DSP_DFT_F32            10u
    #define SERV_DSP_CASCADE_DF1_Q15    3u   /* IIR filters, use SERV_CHECK_COPROCESSOR */
    #define SERV_DSP_CASCADE_DF1_F32    4u         

            /* COMMAND_SSRV */
    #define SERV_DSP_RUN                0u   /* run = default */
    #define SERV_DSP_INIT               1u   /* */
    #define SERV_DSP_WINDOW             2u    
    #define SERV_DSP_WINDOW_DB          3u    

            /* OPTION_SSRV */
    #define SERV_WAIT_COMP          0u   /* tell to return when processing completed (default) */
    #define SERV_RETASAP            1u   /* return even when init/computation is not finished */

            /* FFT with tables rebuilded */
    #define SERV_DSP_rFFT_Q15           5u   /* RFFT windowing, module, dB , use SERV_CHECK_COPROCESSOR */
    #define SERV_DSP_rFFT_F32           6u
                                   
    #define SERV_DSP_cFFT_Q15           7u   /* cFFT windowing, module, dB */
    #define SERV_DSP_cFFT_F32           8u
                                       



/* --------------------------------------------------------------------------- */
/* GROUP_SSRV = 7/SERV_DEEPL ------------------------------------------------ */
/* --------------------------------------------------------------------------- */

        /* COMMAND_SSRV */

        /* OPTION_SSRV */

        /* FUNCTION_SSRV */
    #define NANOGRAPH_FC                   /* fully connected layer Mat x Vec */
    #define NANOGRAPH_CNN                  /* convolutional NN : 3x3 5x5 fixed-weights */


/* --------------------------------------------------------------------------- */
/* GROUP_SSRV = 8/SERV_MM_AUDIO ------------------------------------------------ */
/* --------------------------------------------------------------------------- */

        /* COMMAND_SSRV */

        /* OPTION_SSRV */

        /* FUNCTION_SSRV */

/* --------------------------------------------------------------------------- */
/* GROUP_SSRV = 9/SERV_MM_IMAGE ------------------------------------------------ */
/* --------------------------------------------------------------------------- */

        /* COMMAND_SSRV */

        /* OPTION_SSRV */

        /* FUNCTION_SSRV */
            // SOBEL

/*
* system subroutines : 
* - IO settings : 
* - Get Time, in different formats, and conversion, extract time-stamps
* - Get Peripheral data : RSSI, MAC/IP address
* - Low-level : I2C string of commands, GPIO, physical address to perpherals
*/
#define PLATFORM_DEEPSLEEP_ENABLED 20u   /* deep-sleep activation is possible when returning from arm_graph_interpreter(NANOGRAPH_RUN..) */
#define PLATFORM_TIME_SET          21u
#define PLATFORM_RTC_SET           22u
#define PLATFORM_TIME_READ         23u
#define PLATFORM_HW_WORD_READ      24u  
#define PLATFORM_HW_WORD_WRITE     25u  
#define PLATFORM_HW_BYTE_READ      26u  
#define PLATFORM_HW_BYTE_WRITE     27u  

//enum error_codes 
#define ERROR_MEMORY_ALLOCATION     1u


/*
    STREAM SERVICES
*/

#define  UNUSED_SRV_MSB  31u
#define  UNUSED_SRV_LSB  16u /* 16 reserved */
#define    INST_SRV_MSB  15u       
#define    INST_SRV_LSB  12u /* 4  instance */
#define   GROUP_SRV_MSB  11u       
#define   GROUP_SRV_LSB   8u /* 4  command family groups under compilation options (DSP, Codec, Stdlib, ..) */
#define COMMAND_SRV_MSB   7u       
#define COMMAND_SRV_LSB   0u /* 8  256 service IDs */


/*
    Up to 16 family of processing extensions "SERVICE_COMMAND_GROUP"
    EXTDSPML EXTMATH EXTSTDLIB
*/

#define EXT_SERVICE_MATH   1u
#define EXT_SERVICE_DSPML  2u
#define EXT_SERVICE_STDLIB 3u



/*================================ STREAM ARITHMETICS DATA/TYPE ====================================================*/
/* types fit in 6bits, arrays start with 0, nanograph_bitsize_of_raw() is identical */


#define NANOGRAPH_DATA_ARRAY  0u // nanograph_data_rray : { 0NNN TT 00 } number, type           
#define NANOGRAPH_FP32        1u //  Seeeeeee.mmmmmmmm.mmmmmmmm..  FP32                  
#define NANOGRAPH_FP64        2u //  Seeeeeee.eeemmmmm.mmmmmmm ...  double               
#define NANOGRAPH_S16         3u //  Sxxxxxxx.xxxxxxxx 2 bytes per data                  
#define NANOGRAPH_S32         4u // one long word                                          
#define NANOGRAPH_S2          5u // Sx two bits per data                                 
#define NANOGRAPH_U2          6u // uu                                                   
#define NANOGRAPH_S4          7u // Sxxx four bits per data                              
#define NANOGRAPH_U4          8u // xxxx                                                 
#define NANOGRAPH_FP4_E2M1    9u // Seem  micro-float [8 .. 64]                          
#define NANOGRAPH_FP4_E3M0   10u // Seee   [8 .. 512]                                    
#define NANOGRAPH_S8         11u //  Sxxxxxxx  eight bits per data                       
#define NANOGRAPH_U8         12u //  xxxxxxxx  ASCII char, numbers..                     
#define NANOGRAPH_FP8_E4M3   13u //  Seeeemmm  NV tiny-float [0.02 .. 448]               
#define NANOGRAPH_FP8_E5M2   14u //  Seeeeemm  IEEE-754 [0.0001 .. 57344]                
#define NANOGRAPH_U16        15u //  xxxxxxxx.xxxxxxxx  Numbers, UTF-16 characters       
#define NANOGRAPH_FP16       16u //  Seeeeemm.mmmmmmmm  half-precision float 1E-4 .. 64K            
#define NANOGRAPH_BF16       17u //  Seeeeeee.mmmmmmmm  bfloat                           
#define NANOGRAPH_S23        18u //  Sxxxxxxx.xxxxxxxx.xxxxxxxx  24bits 3 bytes per data 
#define NANOGRAPH_S23_32     19u //  SSSSSSSS.Sxxxxxxx.xxxxxxxx.xxxxxxx  4 bytes per data
#define NANOGRAPH_U32        20u //  xxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx  UTF-32, ..     
#define NANOGRAPH_CS16       21u //  Sxxxxxxx.xxxxxxxx+Sxxxxxxx.xxxxxxxx (I Q)           
#define NANOGRAPH_CFP16      22u //  Seeeeemm.mmmmmmmm+Seeeeemm.. (I Q)                  
#define NANOGRAPH_S64        23u // long long 8 bytes per data                             
#define NANOGRAPH_U64        24u // unsigned 64 bits                                       
#define NANOGRAPH_CS32       25u //  Sxxxxxxx.xxxxxxxx.xxxxxxxx.xxxxxxxx Sxxxx..         
#define NANOGRAPH_CFP32      26u //  Seeeeeee.mmmmmmmm.mmmmmmmm.m..+Seee..  (I Q)        
#define NANOGRAPH_FP128      27u //  Seeeeeee.eeeeeeee.mmmmmmm ...  quadruple precision  
#define NANOGRAPH_CFP64      28u // fp64 + fp64 (I Q)                                      
#define NANOGRAPH_FP256      29u //  Seeeeeee.eeeeeeee.eeeeemm ...  octuple precision    
#define NANOGRAPH_WGS84      30u // <--LAT 32B--><--LONG 32B-->                          
#define NANOGRAPH_HEXBINARY  31u // UTF-8 lower case hexadecimal byte stream               
#define NANOGRAPH_BASE64     32u // RFC-2045 base64 for xsd:base64Binary XML data          
#define NANOGRAPH_STRING8    33u // UTF-8 string of char terminated by 0                   
#define NANOGRAPH_STRING16   34u // UTF-16 string of char terminated by 0                  

#define LAST_RAW_TYPE    64u /* coded on 6bits RAW_FMT0_LSB */

/* ========================== MINIFLOAT 8bits ======================================*/

// Time constants for algorithm
// MiniFloat 76543210
//           MMMEEEEE x= MMM(0..7) << EEEEE(0..31) = [0..15e9] +/-1
#define MINIF(m,exp) ((uint8_t)((m)<<5 | (exp)))
#define MINIFLOAT2Q31(x) ((((x) & 0xE0)>>5) << ((x) & 0x1F))
#define MULTIPLIER_MSB 7u     
#define MULTIPLIER_LSB 5u
#define EXPONENT_MSB 4u     
#define EXPONENT_LSB 0u

// just for information: OFP8_E4M3 SEEEEMMM x= (sign).(1 + M/8).(2<<(E-7)) =[+/- 240] +/- 0.015625 (2^-6)
// https://en.wikipedia.org/wiki/Minifloat
// 
// 8-bit (1.E4.M3)
// . 000  001	.. 010	.. 011	.. 100	.. 101	.. 110	.. 111
// 0 0000 ..	0	0.001953125	0.00390625	0.005859375	0.0078125	0.009765625	0.01171875	0.013671875
// 0 0001 ..	0.015625	0.017578125	0.01953125	0.021484375	0.0234375	0.025390625	0.02734375	0.029296875
// 0 0010 ..	0.03125	0.03515625	0.0390625	0.04296875	0.046875	0.05078125	0.0546875	0.05859375
// 0 0011 ..	0.0625	0.0703125	0.078125	0.0859375	0.09375	0.1015625	0.109375	0.1171875
// 0 0100 ..	0.125	0.140625	0.15625	0.171875	0.1875	0.203125	0.21875	0.234375
// 0 0101 ..	0.25	0.28125	0.3125	0.34375	0.375	0.40625	0.4375	0.46875
// 0 0110 ..	0.5	0.5625	0.625	0.6875	0.75	0.8125	0.875	0.9375
// 0 0111 ..	1	1.125	1.25	1.375	1.5	1.625	1.75	1.875
// 0 1000 ..	2	2.25	2.5	2.75	3	3.25	3.5	3.75
// 0 1001 ..	4	4.5	5	5.5	6	6.5	7	7.5
// 0 1010 ..	8	9	10	11	12	13	14	15
// 0 1011 ..	16	18	20	22	24	26	28	30
// 0 1100 ..	32	36	40	44	48	52	56	60
// 0 1101 ..	64	72	80	88	96	104	112	120
// 0 1110 ..	128	144	160	176	192	208	224	240
// 0 1111 ..	Inf	NaN	NaN	NaN	NaN	NaN	NaN	NaN
// 1 0000 ..	-0	-0.001953125	-0.00390625	-0.005859375	-0.0078125	-0.009765625	-0.01171875	-0.013671875
// 1 0001 ..	-0.015625	-0.017578125	-0.01953125	-0.021484375	-0.0234375	-0.025390625	-0.02734375	-0.029296875
// 1 0010 ..	-0.03125	-0.03515625	-0.0390625	-0.04296875	-0.046875	-0.05078125	-0.0546875	-0.05859375
// 1 0011 ..	-0.0625	-0.0703125	-0.078125	-0.0859375	-0.09375	-0.1015625	-0.109375	-0.1171875
// 1 0100 ..	-0.125	-0.140625	-0.15625	-0.171875	-0.1875	-0.203125	-0.21875	-0.234375
// 1 0101 ..	-0.25	-0.28125	-0.3125	-0.34375	-0.375	-0.40625	-0.4375	-0.46875
// 1 0110 ..	-0.5	-0.5625	-0.625	-0.6875	-0.75	-0.8125	-0.875	-0.9375
// 1 0111 ..	-1	-1.125	-1.25	-1.375	-1.5	-1.625	-1.75	-1.875
// 1 1000 ..	-2	-2.25	-2.5	-2.75	-3	-3.25	-3.5	-3.75
// 1 1001 ..	-4	-4.5	-5	-5.5	-6	-6.5	-7	-7.5
// 1 1010 ..	-8	-9	-10	-11	-12	-13	-14	-15
// 1 1011 ..	-16	-18	-20	-22	-24	-26	-28	-30
// 1 1100 ..	-32	-36	-40	-44	-48	-52	-56	-60
// 1 1101 ..	-64	-72	-80	-88	-96	-104	-112	-120
// 1 1110 ..	-128	-144	-160	-176	-192	-208	-224	-240
// 1 1111 ..	-Inf	NaN	NaN	NaN	NaN	NaN	NaN	NaN

// 8-bit (1.E3.M4)
//       … 0000	… 0001	… 0010	… 0011	… 0100	… 0101	… 0110	… 0111	… 1000	… 1001	… 1010	… 1011	… 1100	… 1101	… 1110	… 1111
// 0 000 …	0	0.015625	0.03125	0.046875	0.0625	0.078125	0.09375	0.109375	0.125	0.140625	0.15625	0.171875	0.1875	0.203125	0.21875	0.234375
// 0 001 …	0.25	0.265625	0.28125	0.296875	0.3125	0.328125	0.34375	0.359375	0.375	0.390625	0.40625	0.421875	0.4375	0.453125	0.46875	0.484375
// 0 010 …	0.5	0.53125	0.5625	0.59375	0.625	0.65625	0.6875	0.71875	0.75	0.78125	0.8125	0.84375	0.875	0.90625	0.9375	0.96875
// 0 011 …	1	1.0625	1.125	1.1875	1.25	1.3125	1.375	1.4375	1.5	1.5625	1.625	1.6875	1.75	1.8125	1.875	1.9375
// 0 100 …	2	2.125	2.25	2.375	2.5	2.625	2.75	2.875	3	3.125	3.25	3.375	3.5	3.625	3.75	3.875
// 0 101 …	4	4.25	4.5	4.75	5	5.25	5.5	5.75	6	6.25	6.5	6.75	7	7.25	7.5	7.75
// 0 110 …	8	8.5	9	9.5	10	10.5	11	11.5	12	12.5	13	13.5	14	14.5	15	15.5
// 0 111 …	Inf	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN
// 1 000 …	−0	−0.015625	−0.03125	−0.046875	−0.0625	−0.078125	−0.09375	−0.109375	−0.125	−0.140625	−0.15625	−0.171875	−0.1875	−0.203125	−0.21875	−0.234375
// 1 001 …	−0.25	−0.265625	−0.28125	−0.296875	−0.3125	−0.328125	−0.34375	−0.359375	−0.375	−0.390625	−0.40625	−0.421875	−0.4375	−0.453125	−0.46875	−0.484375
// 1 010 …	−0.5	−0.53125	−0.5625	−0.59375	−0.625	−0.65625	−0.6875	−0.71875	−0.75	−0.78125	−0.8125	−0.84375	−0.875	−0.90625	−0.9375	−0.96875
// 1 011 …	−1	−1.0625	−1.125	−1.1875	−1.25	−1.3125	−1.375	−1.4375	−1.5	−1.5625	−1.625	−1.6875	−1.75	−1.8125	−1.875	−1.9375
// 1 100 …	−2	−2.125	−2.25	−2.375	−2.5	−2.625	−2.75	−2.875	−3	−3.125	−3.25	−3.375	−3.5	−3.625	−3.75	−3.875
// 1 101 …	−4	−4.25	−4.5	−4.75	−5	−5.25	−5.5	−5.75	−6	−6.25	−6.5	−6.75	−7	−7.25	−7.5	−7.75
// 1 110 …	−8	−8.5	−9	−9.5	−10	−10.5	−11	−11.5	−12	−12.5	−13	−13.5	−14	−14.5	−15	−15.5
// 1 111 …	−Inf	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN	NaN


                       /* |123456789|123456789|123456789|123456789|123456789|123456789|123 */
// NANOGRAPH_TIME16    39 /* ssssssssssssqqqq q14.2   1 hour + 8mn +/- 0.0625 */
// NANOGRAPH_TIME16D   40 /* qqqqqqqqqqqqqqqq q15 [s] time difference +/- 15us */
// NANOGRAPH_TIME32    41 /* ssssssssssssssssssssssssssssqqqq q28.4  [s] (8.5 years +/- 0.0625s) */ 
// NANOGRAPH_TIME32D   42 /* ssssssssssssssssqqqqqqqqqqqqqqqq q17.15 [s] (36h, +/- 30us) time difference */   
// NANOGRAPH_TIME64    43 /* 0000ssssssssssssssssssssssssssssssssqqqqqqqqqqqqqqqqqqqqqqqqqqqq q32.28 [s] 140 Y +Q28 [s] */   
// NANOGRAPH_TIME64MS  44 /* 0010000000000000000000mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm u42 [ms] 140 years */   
// 
// NANOGRAPH_TIME64ISO 45 /* 010..YY..YY..YY..YY..MM..MM..DD..DD..SS..SS.....offs..MM..MM..MM ISO8601 signed offset 2024-05-04T21:12:02+07:00  */   
// *  Local time in BINARY bit-fields : years/millisecond, WWW=day of the week 
// *  (0=Sunday, 1=Monday..)
// *      COVESA allowed formats : ['YYYY_MM_DD', 'DD_MM_YYYY', 'MM_DD_YYYY', 'YY_MM_DD', 'DD_MM_YY', 'MM_DD_YY']
// *  FEDCBA987654321 FEDCBA987654321 FEDCBA987654321 FEDCBA9876543210
// *  _________________________.YY.YY.YY.YY.MMM.DDDD.SSSSS.MM.MM.MM.WW


/*============================ BIT-FIELDS MANIPULATIONS ============================*/
/*
 *  stream constants / Macros.
 */
 
// We define a preprocessor macro that will allow us to add padding
// to a data structure in a way that helps communicate our intent.
// Example : 
//   struct alignas(4) Pixel {
//       char R, G, B;
//       PADDING_BYTES(1);
//   };
//#define CONCATENATE_(a, b) a##b
//#define CONCATENATE(a, b) CONCATENATE_(a, b)
//#define PADDING_BYTES(N) char CONCATENATE(PADDING_MACRO__, __COUNTER__)[N]

#define SHIFT_SIZE(base,shift) ((base) << ((shift) << 2));           

#define MIN(a, b) (((a) > (b))?(b):(a))
#define MAX(a, b) (((a) < (b))?(b):(a))
#define ABS(a) (((a)>0)? (a):-(a))

#define MAXINT32 0x7FFFFFFFL
#define MEMCPY(dst,src,n) {uint32_t imcpy; for(imcpy=0;imcpy<(n);imcpy++){((dst)[imcpy])=((src)[imcpy]);}}
#define MEMSET(dst,c,n) {uint32_t i; uint8_t *pt8=(uint8_t *)(dst); for(i=0;i<(n);i++){(pt8[i])=(c);} }


/* bit-field manipulations */
#define CREATE_MASK(msb, lsb)               ((uint32_t)((U(1L) << ((msb) - (lsb) + U(1L))) - U(1L)) << (lsb))
#define MASK_BITS(arg, msb, lsb)            ((arg) & CREATE_MASK(msb, lsb))
#define EXTRACT_BITS(arg, msb, lsb)         (MASK_BITS(arg, msb, lsb) >> (lsb))
#define INSERT_BITS(arg, msb, lsb, value) \
    ((arg) = ((arg) & ~CREATE_MASK(msb, lsb)) | (((value) << (lsb)) & CREATE_MASK(msb, lsb)))
#define MASK_FIELD(arg, field)              MASK_BITS((arg), field##_MSB, field##_LSB)

#define EXTRACT_FIELD(arg, field)           U(EXTRACT_BITS((U(arg)), field##_MSB, field##_LSB))
#define RD(arg, field) U(EXTRACT_FIELD(arg, field))

#define INSERT_FIELD(arg, field, value)     INSERT_BITS((arg), field##_MSB, field##_LSB, value)
#define ST(arg, field, value) INSERT_FIELD((arg), field, U(value)) 

#define LOG2NBYTESWORD32 2 
#define NBYTESWORD32 (1<<LOG2NBYTESWORD32)

// replaced #define LINADDR_UNIT_BYTE   1
//      #define LINADDR_UNIT_W32    4
//      #define LINADDR_UNIT_EXTD  64

#define SET_BIT(arg, bit)   ((arg) |= (U(1) << U(bit)))
#define CLEAR_BIT(arg, bit) ((arg) = (arg) & U(~(U(1) << U(bit))))
#define TEST_BIT(arg, bit)  (U(0) != (U(arg) & (U(1) << U(bit))))
#define READ_BIT(arg, bit)  (U(arg) & (U(1) << U(bit)))

#define FLOAT_TO_INT(x) ((x)>=0.0f?(int)((x)+0.5f):(int)((x)-0.5f))

/* DSP processing data types */
#define samp_t int16_t  /* default size of input samples = 16bits */
#define coef_t int16_t
#define accu_t int32_t  /* accumulator with software pre-shifter */
#define data_t int32_t 
#define iidx_t int32_t  /* index of the loop */

#define samp_f float
#define coef_f float
#define accu_f float
#define data_f float

#define f32_t float
#define s16_t int16_t
#define s32_t int32_t

#define CONSTPOW2_FP32(n)  (float_t)(1<<(n))         /* convert a constant to float */
#define CONSTPOW10_FP32(n) (float_t)(powf(10,(n)))  


/* see arm_nanograph_router.c and other nodes */
typedef union conv_int32_fp32_t
{   float     f;
    uint32_t  u;
} conv_int32_fp32_t;

#endif /* cNANOGRAPH_COMMON_CONST_H */
/*
 * -----------------------------------------------------------------------
 */
#ifdef __cplusplus
}
#endif
    

