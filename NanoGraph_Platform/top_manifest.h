/* ----------------------------------------------------------------------
 * top_manifest.h
 * -------------------------------------------------------------------- */


 /*----- PLATFORM DEFINITIONS FOR "RA8E1" ------------------------------------------------------------------------*/

#undef RTOS_USED

#define PLATFORM_RA8E1FPB
#define PLATFORM_ARCH_32BIT
//#define PLATFORM_ARCH_64BIT

#define TIME_BASE_1MS                       /* SYSTICK time base */
#define PROCESSOR_CLOCK 350000000L          /* SYSTICK clock */

#define NANOGRAPH_NB_INSTANCE 1
#define PLATFORM_PROCESSOR 1            
#define PLATFORM_ARCHITECTURE 1            


#define GRAPH_FROM_PLATFORM             /* the graph comes from the platform (internal), otherwise from the application  */

/*------ Floating point allowed ------*/
#define NANOGRAPH_FLOAT_ALLOWED                /* selection of integer processing with float_t and double_t */

/* max number of nodes installed at compilation time */
#define NB_NODE_ENTRY_POINTS 30

/* max number of application callbacks used from NODE and scripts */
#define MAX_NB_APP_CALLBACKS 4

#define MULTIPROCESSING                 /* enable memory flush conditional codes */
//#define MEMID0_CACHED                   /* ARC descriptors in MEMID0,  default is uncached (ex. Cortex-M0) */

#define CACHE_LINE_BYTE_LENGTH 0        /* 0 for Cortex-M armv6/v7/v8-m */
//#define CACHE_LINE_BYTE_LENGTH 32       /* 32Bytes (CM7/CM55) */
//#define CACHE_LINE_BYTE_LENGTH 64       /* 64bytes for Cortex-A armv8/v9 */

/*
 * --- maximum number of processors using STREAM in parallel - read by the graph compiler
 */
#define NANOGRAPH_NB_PROCESSES 1           /* Max number of process(or) reeading the graph simultaneously */

#define NANOGRAPH_MAXNB_THREADS_PER_PROC 1
#define NANOGRAPH_MAXNB_PROCESSOR_PER_ARCH 2
#define NANOGRAPH_NB_ARCHITECTURES 1

 // MEMORY BANKS

#define MBANK_GRAPH     0               /* share graph base address */
#define MBANK_DMEMFAST  1               /* not shared DTCM Cortex-M/LLRAM Cortex-R, swapped between NODE calls if static */

#define SIZE_MBANK_DMEM_EXT     5000    /* general purpose          */
#define SIZE_MBANK_DTCM          100        /* simulates DTCM           */
#define SIZE_MBANK_ITCM          100        /* simulates ITCM           */
#define SIZE_MBANK_RETENTION     100    /* simulates retention      */


        /* warning : changing the indexes impacts the "top_graph_interface" of each graph.txt */
#define IO_PLATFORM_DATA_SINK        0 

#define IO_PLATFORM_DATA_IN_0         1
#define IO_PLATFORM_DATA_IN_1         2
#define IO_PLATFORM_DATA_OUT_0        3
#define IO_PLATFORM_DATA_OUT_1        4
#define IO_PLATFORM_SENSOR_IN_0       5
#define IO_PLATFORM_SENSOR_IN_1       6
#define IO_PLATFORM_SENSOR_IN_2       7
#define IO_PLATFORM_SENSOR_IN_3       8
#define IO_PLATFORM_TIMER_0           9
#define IO_PLATFORM_TIMER_1           10
#define IO_PLATFORM_UI_IN_0           11
#define IO_PLATFORM_UI_IN_1           12
#define IO_PLATFORM_UI_IN_2           13
#define IO_PLATFORM_UI_IN_3           14
#define IO_PLATFORM_UI_OUT_0          15
#define IO_PLATFORM_UI_OUT_1          16
#define IO_PLATFORM_UI_OUT_2          17
#define IO_PLATFORM_UI_OUT_3          18
#define IO_PLATFORM_SERIAL_IN_0       19
#define IO_PLATFORM_SERIAL_OUT_0      20
#define IO_PLATFORM_ANALOG_IN_0       21
#define IO_PLATFORM_ANALOG_OUT_0      22
#define IO_PLATFORM_AUDIO_IN_0        23
#define IO_PLATFORM_AUDIO_IN_1        24
#define IO_PLATFORM_AUDIO_IN_2        25
#define IO_PLATFORM_AUDIO_OUT_0       26
#define IO_PLATFORM_AUDIO_OUT_1       27
#define IO_PLATFORM_AUDIO_OUT_2       28
#define IO_PLATFORM_2D_IN_0           30
#define IO_PLATFORM_2D_IN_1           31
#define IO_PLATFORM_2D_OUT_0          32
#define IO_PLATFORM_2D_OUT_1          33

#define MAX_NBGRAPHIO (1 + IO_PLATFORM_2D_OUT_1)

//#define LAST_IO_FUNCTION_PLATFORM (IO_PLATFORM_DATA_OUT_0+1)  /* table of platform_io[io_al_idx] */

//#define MAX_IO_FUNCTION_PLATFORM 128     /* table of platform_io[io_al_idx] */

/*===========================================================================
 in platform_platform.c : 
    void platform_specific_long_offset(intptr_t long_offset[])
        long_offset[MBANK_GRAPH]    = (const intptr_t)&(MEXT[10]); 
        long_offset[MBANK_DMEMFAST] = (const intptr_t)&(TCM1[10]); 
*/

#define CODE_ARM_STREAM_SCRIPT          /* byte-code interpreter, index "arm_stream_script_INDEX" */
#define CODE_ARM_STREAM_FILTER          /* cascade of DF1 filters */
#define CODE_SIGP_STREAM_DETECTOR       /* estimates peaks/floor of the mono input and triggers a flag on high SNR */



/*----- SERVICES ENABLED -----------------------------------------------------------------------*/
// SERV_GROUP_INTERNAL           /* 0  internal : Semaphores, DMA, Clocks */
    //#define PLATFORM_SERV_INTERNAL_SLEEP_CONTROL                              0
    //#define PLATFORM_SERV_INTERNAL_CPU_CLOCK_UPDATE                           1
    //#define PLATFORM_SERV_INTERNAL_READ_MEMORY                                2
    //#define PLATFORM_SERV_INTERNAL_READ_MEMORY_FAST_MEM_ADDRESS               3
    //#define PLATFORM_SERV_INTERNAL_SERIAL_COMMUNICATION                       4
    //#define PLATFORM_SERV_INTERNAL_MUTUAL_EXCLUSION_WR_BYTE_AND_CHECK_MP      5
    //#define PLATFORM_SERV_INTERNAL_MUTUAL_EXCLUSION_RD_BYTE_MP                6
    //#define PLATFORM_SERV_INTERNAL_MUTUAL_EXCLUSION_WR_BYTE_MP                7
    //#define PLATFORM_SERV_INTERNAL_MUTUAL_EXCLUSION_CLEAR_BIT_MP              8
    //#define PLATFORM_SERV_INTERNAL_READ_TIME                                  9
    //#define PLATFORM_SERV_INTERNAL_READ_TIME64                               10
    //#define PLATFORM_SERV_INTERNAL_READ_TIME32                               11
    //#define PLATFORM_SERV_INTERNAL_READ_TIME16                               12
    //#define PLATFORM_SERV_INTERNAL_KEYEXCHANGE                               13

// SERV_GROUP_SCRIPT             /* 1  script : Node parameters  */

//#undef SERV_GROUP_CONVERSION         /* 2  Compute : raw conversions */
    //#undef PLATFORM_SERV_SERV_CONVERSION_INT16_FP32

//#undef SERV_GROUP_STDLIB             /* 3  Compute : malloc, string */
    //#undef PLATFORM_SERV_NANOGRAPH_ATOF    
    //#undef PLATFORM_SERV_NANOGRAPH_ATOI    
    //#undef PLATFORM_SERV_NANOGRAPH_MEMSET  
    //#undef PLATFORM_SERV_NANOGRAPH_STRCHR  
    //#undef PLATFORM_SERV_NANOGRAPH_STRLEN  
    //#undef PLATFORM_SERV_NANOGRAPH_STRNCAT 
    //#undef PLATFORM_SERV_NANOGRAPH_STRNCMP 
    //#undef PLATFORM_SERV_NANOGRAPH_STRNCPY 
    //#undef PLATFORM_SERV_NANOGRAPH_STRSTR  
    //#undef PLATFORM_SERV_NANOGRAPH_STRTOK  
    //#undef PLATFORM_SERV_NANOGRAPH_FREE    
    //#undef PLATFORM_SERV_NANOGRAPH_MALLOC  

//#undef SERV_GROUP_MATH               /* 4  math.h */
    //#undef PLATFORM_SERV_SERV_SQRT_Q15  
    //#undef PLATFORM_SERV_SERV_SQRT_F32  
    //#undef PLATFORM_SERV_SERV_LOG_Q15   
    //#undef PLATFORM_SERV_SERV_LOG_F32   
    //#undef PLATFORM_SERV_SERV_SINE_Q15  
    //#undef PLATFORM_SERV_SERV_SINE_F32  
    //#undef PLATFORM_SERV_SERV_COS_Q15   
    //#undef PLATFORM_SERV_SERV_COS_F32   
    //#undef PLATFORM_SERV_SERV_ATAN2_Q15 
    //#undef PLATFORM_SERV_SERV_ATAN2_F32 
    //#undef PLATFORM_SERV_SERV_SORT      

// SERV_GROUP_DSP_ML             /* 5  cmsis-dsp */
    //#undef PLATFORM_SERV_CHECK_COPROCESSOR    /* check for services() */
    //#undef PLATFORM_SERV_SERV_CHECK_END_COMP       /* check completion for the caller */
    //#undef PLATFORM_SERV_SERV_DFT_Q15              /* DFT/Goertzel windowing, module, dB */
    //#undef PLATFORM_SERV_SERV_DFT_F32            

    #define PLATFORM_SERV_DSP_CASCADE_DF1_Q15           /* IIR filters, use SERV_CHECK_COPROCESSOR */
#define PLATFORM_SERV_DSP_CASCADE_DF1_Q15_ESS       /*  error spectral shaping */          
    //#define PLATFORM_SERV_DSP_CASCADE_DF1_F32         /* take the default implementation */

    //#undef PLATFORM_SERV_SERV_WINDOW                
    //#undef PLATFORM_SERV_SERV_WINDOW_DB             
    //#undef PLATFORM_SERV_SERV_rFFT_Q15             /* RFFT windowing, module, dB , use SERV_CHECK_COPROCESSOR */
    //#undef PLATFORM_SERV_SERV_rFFT_F32             /* default FFT with tables rebuilded */
    //#undef PLATFORM_SERV_SERV_cFFT_Q15             /* cFFT windowing, module, dB */
    //#undef PLATFORM_SERV_SERV_cFFT_F32           

//#undef SERV_GROUP_DEEPL              /* 6  cmsis-nn */
    //#undef PLATFORM_SERV_NANOGRAPH_FC                 /* fully connected layer Mat x Vec */
    //#undef PLATFORM_SERV_NANOGRAPH_CNN                /* convolutional NN : 3x3 5x5 fixed-weights */

//#undef PLATFORM_SERV_SERV_GROUP_MM_AUDIO           /* 7 speech/audio processing */

//#undef PLATFORM_SERV_SERV_GROUP_MM_IMAGE           /* 8 image processing */

//optimization - fully - connected 
//optimization - conv2d

///* conditional compilation */
//#define NANOGRAPH_PLATFORM_SERVICES        /* call the platform service with its fast libraries w/wo accelerators */
    //#undef PLATFORM_SERV_SERV_STDLIB
    //#undef PLATFORM_SERV_SERV_EXTMATH
    //#undef PLATFORM_SERV_SERV_EXTAUDIO
    //#undef PLATFORM_SERV_SERV_EXTIMAGE



/*
    write memory barrier is executed before cache clean    : saveto(*); WMB; CLEAN(*); 
    read memory barrier is executed after cache invalidate : INVAL(*); RMB; return(*);
 */
#define DATA_MEMORY_BARRIER
#define INSTRUCTION_SYNC_BARRIER
#define CLEAN_BUFFER_1LINE(addr)
#define INVALIDATE_BUFFER_1LINE(addr) 

/* void SCB_CleanInvalidateDCache_by_Addr (volatile void * addr, int32_t dsize) */
#define CLEAN_BUFFER_RANGE(addr,dsize)
/* void SCB_InvalidateDCache_by_Addr (volatile void * addr, int32_t dsize) */
#define INVALIDATE_BUFFER_RANGE(addr,dsize)

#ifdef MULTIPROCESSING
#define DATA_MEMORY_BARRIER //DMB()
#define INSTRUCTION_SYNC_BARRIER //ISB()
#else
#define DATA_MEMORY_BARRIER 
#define INSTRUCTION_SYNC_BARRIER
#endif


#define WR_BYTE_MP_(address,x) { *(volatile uint8_t *)(address) = (x); DATA_MEMORY_BARRIER; }
#define RD_BYTE_MP_(x,address) { DATA_MEMORY_BARRIER; (x) = *(volatile uint8_t *)(address);}
#define CLEAR_BIT_MP(arg, bit) {((arg) = U(arg) & U(~(U(1) << U(bit)))); DATA_MEMORY_BARRIER; } 

