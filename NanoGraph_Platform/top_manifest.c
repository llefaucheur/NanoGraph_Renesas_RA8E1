/* ----------------------------------------------------------------------
 * top_manifest.c
 * -------------------------------------------------------------------- */


#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../nanograph_interpreter.h"
#include "top_manifest.h"

/*------------------------------------------------------------------------------------
    ALL THE NODES OF ALL PROCESSORS, AND THEIR INDEX 
-------------------------------------------------------------------------------------*/
extern p_nanograph_node NanoGraph_null_task;          /*  0 */
extern p_nanograph_node NanoGraph_script;             /*  1  #define NanoGraph_script_index 1 */

  extern p_nanograph_node arm_nanograph_filter;         /*  4 */

  extern p_nanograph_node sigp_nanograph_detector;      /*  8 */



const p_nanograph_node node_entry_points[NB_NODE_ENTRY_POINTS] =
{
    /*  0 */ (p_nanograph_node)&NanoGraph_null_task,       /* ID | PROC ARCH    */
    /*  1 */ (p_nanograph_node)&NanoGraph_script,             /*  1 |   0   0    */
    /*  2 */ 0, // (p_nanograph_node)&arm_nanograph_router,         /*  2 |   0   1    */
    /*  3 */ 0, // (p_nanograph_node)&arm_nanograph_amplifier,      /*  3 |   0   1    */
    /*  4 */ (p_nanograph_node)&arm_nanograph_filter,         /*  4 |   0   1    */
    /*  5 */ 0, // (p_nanograph_node)&arm_nanograph_modulator,      /*  5 |   0   1    */
    /*  6 */ 0, // (p_nanograph_node)&arm_nanograph_demodulator,    /*  6 |   0   1    */
    /*  7 */ 0, // (p_nanograph_node)&arm_nanograph_filter2D,       /*  7 |   0   1    */
    /*  8 */ (p_nanograph_node)&sigp_nanograph_detector,      /*  8 |   0   1    */
    /*  9 */ 0, // (p_nanograph_node)&sigp_nanograph_detector2D,    /*  9 |   0   1    */
    /* 10 */ 0, // (p_nanograph_node)&sigp_nanograph_resampler,     /* 10 |   0   1    */
    /* 11 */ 0, // (p_nanograph_node)&sigp_nanograph_compressor,    /* 11 |   0   1    */
    /* 12 */ 0, // (p_nanograph_node)&sigp_nanograph_decompressor,  /* 12 |   0   1    */
    /* 13 */ 0, // (p_nanograph_node)&bitbank_JPEGENC,              /* 13 |   0   1    */
    /* 14 */ 0, // (p_nanograph_node)&TjpgDec,                      /* 14 |   0   1    */
};


/* 
    IO interfaces functions to the possible streams of the platform
*/
extern void data_sink     (uint32_t, nanograph_xdmbuffer_t *);  // 0
extern void io_data_in_0        (uint32_t, nanograph_xdmbuffer_t *);  //  1
extern void io_data_in_1        (uint32_t, nanograph_xdmbuffer_t *);  //  2
extern void io_data_out_0       (uint32_t, nanograph_xdmbuffer_t *);  //  3
extern void io_data_out_1       (uint32_t, nanograph_xdmbuffer_t *);  //  4
extern void io_sensor_in_0      (uint32_t, nanograph_xdmbuffer_t *);  //  5
//extern void io_sensor_in_1      (uint32_t, nanograph_xdmbuffer_t *);  //  6
//extern void io_sensor_in_2      (uint32_t, nanograph_xdmbuffer_t *);  //  7
//extern void io_sensor_in_3      (uint32_t, nanograph_xdmbuffer_t *);  //  8
extern void io_timer_0          (uint32_t, nanograph_xdmbuffer_t *);  //  9
//extern void io_timer_1          (uint32_t, nanograph_xdmbuffer_t *);  // 10
extern void io_ui_in_0          (uint32_t, nanograph_xdmbuffer_t *);  // 11
//extern void io_ui_in_1          (uint32_t, nanograph_xdmbuffer_t *);  // 12
//extern void io_ui_in_2          (uint32_t, nanograph_xdmbuffer_t *);  // 13
//extern void io_ui_in_3          (uint32_t, nanograph_xdmbuffer_t *);  // 14
extern void io_ui_out_0         (uint32_t, nanograph_xdmbuffer_t *);  // 15
//extern void io_ui_out_1         (uint32_t, nanograph_xdmbuffer_t *);  // 16
//extern void io_ui_out_2         (uint32_t, nanograph_xdmbuffer_t *);  // 17
//extern void io_ui_out_3         (uint32_t, nanograph_xdmbuffer_t *);  // 18
extern void io_serial_in_0      (uint32_t, nanograph_xdmbuffer_t *);  // 19
//extern void io_serial_out_0     (uint32_t, nanograph_xdmbuffer_t *);  // 20
extern void io_analog_in_0      (uint32_t, nanograph_xdmbuffer_t *);  // 21
//extern void io_analog_out_0     (uint32_t, nanograph_xdmbuffer_t *);  // 22
extern void io_audio_in_0       (uint32_t, nanograph_xdmbuffer_t *);  // 23
//extern void io_audio_in_1       (uint32_t, nanograph_xdmbuffer_t *);  // 24
//extern void io_audio_in_2       (uint32_t, nanograph_xdmbuffer_t *);  // 25
extern void io_audio_out_0      (uint32_t, nanograph_xdmbuffer_t *);  // 26
//extern void io_audio_out_1      (uint32_t, nanograph_xdmbuffer_t *);  // 27
//extern void io_audio_out_2      (uint32_t, nanograph_xdmbuffer_t *);  // 28
extern void io_2d_in_0          (uint32_t, nanograph_xdmbuffer_t *);  // 30
//extern void io_2d_in_1          (uint32_t, nanograph_xdmbuffer_t *);  // 31
//extern void io_2d_out_0         (uint32_t, nanograph_xdmbuffer_t *);  // 32
//extern void io_2d_out_1         (uint32_t, nanograph_xdmbuffer_t *);  // 33

/*  
    global read-only index to the current graph interpreter instance 
*/
uint8_t platform_io_instance_idx;
nanograph_instance_t* platform_io_callback_parameter;
extern uintptr_t all_ptr_instances[NANOGRAPH_NB_INSTANCE];




/* ------------------------------------------------
       Memory map of the platform
       see top_manifest_platform.txt
  ------------------------------------------------ */
uint8_t MEXT[SIZE_MBANK_DMEM_EXT];  
uint8_t DTCM[SIZE_MBANK_DTCM];
uint8_t ITCM[SIZE_MBANK_ITCM];
uint8_t BACKUP[SIZE_MBANK_RETENTION];

const uint8_t* long_offset[MAX_PROC_MEMBANK] =
{ &(MEXT[0]), &(DTCM[0]), &(ITCM[0]), &(BACKUP[0]) };


uint8_t one_file_is_closed;         /* flag used to exit */

/*---------------------------------------------------------
  IO AFFINITY WITH PROCESSOR  
  ---------------------------------------------------------*/
#define LAST_IO_FUNCTION_PLATFORM (IO_PLATFORM_DATA_OUT_0+1)  /* table of platform_io[io_al_idx] */

const p_io_function_ctrl platform_io[] =
{   
    data_sink           ,// 0
    io_data_in_0        ,// 1
    io_data_in_1        ,// 2
    io_data_out_0       ,// 3
    io_data_out_1       ,// 4 
    io_sensor_in_0      ,// 5   
    0               , //io_sensor_in_1      ,// 6  
    0               , //io_sensor_in_2      ,// 7   
    0               , //io_sensor_in_3      ,// 8 
    io_timer_0          ,// 9 
    0               , //io_timer_1          ,// 10  
    io_ui_in_0          ,// 11
    0               , //io_ui_in_1          ,// 12
    0               , //io_ui_in_2          ,// 13
    0               , //io_ui_in_3          ,// 14
    io_ui_out_0         ,// 15
    0               , //io_ui_out_1         ,// 16
    0               , //io_ui_out_2         ,// 17
    0               , //io_ui_out_3         ,// 18
    io_serial_in_0      ,// 19
    0               , //io_serial_out_0     ,// 20
    io_analog_in_0      ,// 21
    0               , //io_analog_out_0     ,// 22
    io_audio_in_0       ,// 23
    0               , //io_audio_in_1       ,// 24
    0               , //io_audio_in_2       ,// 25
    io_audio_out_0     , // 26
    0               , //io_audio_out_1      ,// 27
    0               , //io_audio_out_2      ,// 28
    0               , //io_audio_out_3      ,// 29
    io_2d_in_0          ,// 30
    0               , //io_2d_in_1          ,// 31
    0               , //io_2d_out_0         ,// 32
    0               , //io_2d_out_1         ,// 33
};


/*
    Callback for NODE and scripts (SYSCALL) 
        system call (0..7) 
        application callbacks (8..15))
        ARC debug activities (ARC_APP_CALLBACK1)
    Use-case :
        deep-sleep proposal from the scheduler
        event detection trigger, software timers
        metadata sharing from script
        allow execution of nano_graph_interpreter(NANOGRAPH_RESET..) after a graph reload
*/
const p_nanograph_services_t application_callbacks[MAX_NB_APP_CALLBACKS] =
{   (void*)0,
    (void*)0,
    (void*)0,
    (void*)0
};

/*==========================================================================================================================*/
/*
*   the graph to be executed @@@
*/
const uint32_t graph_platform[] =
{
    #include "graphs/graph_bin.txt"
};



/*
 *  TIME (see nanograph_time64 definition)
 *
 *  "nanograph_time64" example of implementation using a global variable
 *  FEDCBA987654321 FEDCBA987654321 FEDCBA987654321 FEDCBA9876543210
 *  ____ssssssssssssssssssssssssssssssssqqqqqqqqqqqqqqqqqqqqqqqqqqqq q32.28 [s]  140 Y + Q28 [s]
 *  systick increment for  1ms =  0x00041893 =  1ms x 2^28
 *  systick increment for 10ms =  0x0028F5C2 = 10ms x 2^28
 *
 * Other implementation rely on an HW timer (RP2040)
*/

uint64_t global_nanograph_time64;


/*
    Table of pairs {node + parameter address} for parameter updates from the application
*/
uintptr_t new_node_parameters[(1 + MAX_NB_PENDING_PARAM_UPDATES) * 2] =
{   0, 0,  // [node idx in the graph ; physical address to the parameters in "boot" format]
    0, 0,  // [node idx; parameter address]
    // .. 
    0, 0,  // end of the list 
};



/*==========================================================================================================================*/

uint32_t* get_graph_address(uint32_t graph_idx)
{
    return (uint32_t*)graph_platform;
}

/*==========================================================================================================================*/
/**
  @brief            Shares the pl
  atform-specific data for the initialization of the interpreter instance
  @param[in/out]    none
  @return           int
 */

void platform_init_specific(NanoGraph_init_t* data)
{
    /* flag detecting end of simulations for platforms using files */
    {   extern uint8_t one_file_is_closed;
        one_file_is_closed = 0;
    }

    /* systick simulation */
    {   extern void SysTickSetup(void);
        SysTickSetup();
    }
#ifdef GRAPH_FROM_PLATFORM
    data->graph = get_graph_address(0);
#endif

    data->long_offset = (uint8_t**)long_offset;           // pointer to "long_offset[MAX_PROC_MEMBANK]"

    data->node_entry_points = (p_nanograph_node)node_entry_points;             // list of nodes
    data->platform_io = (p_io_function_ctrl)platform_io;                     // list of IO functions
    data->new_parameters = (uintptr_t)new_node_parameters;                   // list of pairs [offset; parameter address]

    data->procID = PLATFORM_PROCESSOR;
    data->archID = PLATFORM_ARCHITECTURE;
}


/*==========================================================================================================================*/
/* here: test the need for memory recovery/swap
    does the application modified the memory banks used by the graph ? */
void memory_swap(nanograph_instance_t* S)
{
    static int i;
    i = 1;
    return;
    //uint8_t memBankBitFields[(7 + MAX_PROC_MEMBANK) / 8];
}

/*==========================================================================================================================*/
