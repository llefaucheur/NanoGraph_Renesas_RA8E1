/* ----------------------------------------------------------------------
 * Project:      NanoGraph
 * Title:        graph_test_scheduler.c
 * Description:  filters
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


#include "../top_manifest_included.h"

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef RTOS_USED
 #include "FreeRTOS.h"
 #include "task.h"
#endif

#include "../nanograph_common.h"
#include "../nanograph_interpreter.h"

extern void NanoGraph_io_ack (uint8_t graph_hwio_idx, void *data, uintptr_t size);
extern void graph_test_scheduler(uint64_t time64);

#define BareMetalTaskHandle0_mask (1 << 0)
#define BareMetalTaskHandle1_mask (1 << 1)
#define BareMetalTaskHandle2_mask (1 << 2)

#define AUDIOFRAMESIZE 16
#define AUDIOSIZE 160000
const int16_t  tst_a_in_0 [AUDIOSIZE/2] =
{
    #include "io_test_audio_in_0.txt"
};
const int16_t  tstaudio_in_0 [] = 
{
    #include "io_test_audio_in_0.txt"
};
const int16_t  tstaudio_in_1 [] = 
{
    #include "io_test_audio_in_0.txt"
};

#define UIOUT0SIZE 16
uint32_t tst_ui_out_0[UIOUT0SIZE];


#ifdef RTOS_USED
  extern TaskHandle_t new_thread0;
  extern TaskHandle_t new_thread1;
  extern TaskHandle_t new_thread2;
#else
  uint32_t BareMetalTaskHandle;
#endif

extern void graph_test_scheduler(uint64_t time64);


/*  graph_interpreter_time64" using a global variable
 *  FEDCBA987654321 FEDCBA987654321 FEDCBA987654321 FEDCBA9876543210
 *  ____ssssssssssssssssssssssssssssssssqqqqqqqqqqqqqqqqqqqqqqqqqqqq q32.28 [s]  140 Y + Q28 [s]
 *
 *  increments for  1ms systick =  0x00041893 =  1ms x 2^28
 *  increments for 10ms systick =  0x0028F5C2 = 10ms x 2^28
 */

#define GTIMESEC(x) ((uint64_t)((x)*((float)(1L<<28))))

typedef struct 
{
    uint8_t IOIDX;
    uint8_t  *data;
    uint32_t buffer_size;
    uint32_t frame_length;
    uint64_t period;
    uint8_t instance_affinity;   
    
} io_test_struct_t;

const io_test_struct_t ios[MAX_NBGRAPHIO] =
{ //   IO                       data            buffer_size    frame size       period      instance_affinity
  // ; ----------------------------------------------------------------------
  // format_index            0
  // format_frame_length     32; 1ms mono 16b 16kHz
  // format_index            1
  // format_frame_length     2; io_platform_gpio_out_1
  // ; ----------------------------------------------------------------------
  { IO_PLATFORM_DATA_IN_0    ,        0,              0,              0,              0,          0 }, // DATA_IN_0         1
  { IO_PLATFORM_DATA_IN_1    ,        0,              0,              0,              0,          0 }, // DATA_IN_1         2
  { IO_PLATFORM_DATA_OUT_0   ,        0,              0,              0,              0,          0 }, // DATA_OUT_0        3
  { IO_PLATFORM_DATA_OUT_1   ,        0,              0,              0,              0,          0 }, // DATA_OUT_1        4
  { IO_PLATFORM_SENSOR_IN_0  , (uint8_t*)tst_a_in_0, AUDIOSIZE, AUDIOFRAMESIZE, GTIMESEC(0.002),  0 }, // SENSOR_IN_0       5
  { IO_PLATFORM_SENSOR_IN_1  ,        0,              0,              0,              0,          0 }, // SENSOR_IN_1       6
  { IO_PLATFORM_SENSOR_IN_2  ,        0,              0,              0,              0,          0 }, // SENSOR_IN_2       7
  { IO_PLATFORM_SENSOR_IN_3  ,        0,              0,              0,              0,          0 }, // SENSOR_IN_3       8
  { IO_PLATFORM_TIMER_0      ,        0,              0,              0,              0,          0 }, // TIMER_0           9
  { IO_PLATFORM_TIMER_1      ,        0,              0,              0,              0,          0 }, // TIMER_1           10
  { IO_PLATFORM_UI_IN_0      ,        0,              0,              0,              0,          0 }, // UI_IN_0           11
  { IO_PLATFORM_UI_IN_1      ,        0,              0,              0,              0,          0 }, // UI_IN_1           12
  { IO_PLATFORM_UI_IN_2      ,        0,              0,              0,              0,          0 }, // UI_IN_2           13
  { IO_PLATFORM_UI_IN_3      ,        0,              0,              0,              0,          0 }, // UI_IN_3           14
  { IO_PLATFORM_UI_OUT_0     ,        0,              0,              0,              0,          0 }, // UI_OUT_0          15
  { IO_PLATFORM_UI_OUT_1     ,        0,              0,              0,              0,          0 }, // UI_OUT_1          16
  { IO_PLATFORM_UI_OUT_2     ,        0,              0,              0,              0,          0 }, // UI_OUT_2          17
  { IO_PLATFORM_UI_OUT_3     ,        0,              0,              0,              0,          0 }, // UI_OUT_3          18
  { IO_PLATFORM_SERIAL_IN_0  ,        0,              0,              0,              0,          0 }, // SERIAL_IN_0       19
  { IO_PLATFORM_SERIAL_OUT_0 ,        0,              0,              0,              0,          0 }, // SERIAL_OUT_0      20
  { IO_PLATFORM_ANALOG_IN_0  ,        0,              0,              0,              0,          0 }, // ANALOG_IN_0       21
  { IO_PLATFORM_ANALOG_OUT_0 ,        0,              0,              0,              0,          0 }, // ANALOG_OUT_0      22
  { IO_PLATFORM_AUDIO_IN_0   ,        0,              0,              0,              0,          0 }, // AUDIO_IN_0        23
  { IO_PLATFORM_AUDIO_IN_1   ,        0,              0,              0,              0,          0 }, // AUDIO_IN_1        24
  { IO_PLATFORM_AUDIO_IN_2   ,        0,              0,              0,              0,          0 }, // AUDIO_IN_2        25
  { IO_PLATFORM_AUDIO_OUT_0  ,        0,              0,              0,              0,          0 }, // AUDIO_OUT_0       26
  { IO_PLATFORM_AUDIO_OUT_1  ,        0,              0,              0,              0,          0 }, // AUDIO_OUT_1       27
  { IO_PLATFORM_AUDIO_OUT_2  ,        0,              0,              0,              0,          0 }, // AUDIO_OUT_2       28
  { IO_PLATFORM_2D_IN_0      ,        0,              0,              0,              0,          0 }, // 2D_IN_0           30
  { IO_PLATFORM_2D_IN_1      ,        0,              0,              0,              0,          0 }, // 2D_IN_1           31
  { IO_PLATFORM_2D_OUT_0     ,        0,              0,              0,              0,          0 }, // 2D_OUT_0          32
  { IO_PLATFORM_2D_OUT_1     ,        0,              0,              0,              0,          0 }, // 2D_OUT_1          33
};                                                                

    

void graph_test_scheduler(uint64_t time64)
{
    static uint64_t io_counter[MAX_NBGRAPHIO];
    static uint32_t read_index[MAX_NBGRAPHIO];
    static uint8_t initialization;
    uint8_t *pt8;
    uint32_t i, threads;
    

    if (0 == initialization)
    {   
        for (i = 1; i < MAX_NBGRAPHIO; i++)
        {	io_counter[i] = time64 + ios[i].period;
        }
        BareMetalTaskHandle = 0;
        initialization = 1;
    }

    /* call the graph scheduler if this time to exchange new data */
    threads = 0;
    for (i = 1; i < MAX_NBGRAPHIO; i++)
    {   
        if (ios[i].frame_length == 0)
        	continue;

        if (time64 > io_counter[i])
        {   
            pt8 = ios[i].data + read_index[i];
            NanoGraph_io_ack(ios[i].IOIDX, pt8, ios[i].frame_length);
            read_index[i] += ios[i].frame_length;
            if (read_index[i] >= (ios[i].buffer_size - ios[i].frame_length))
            {
                extern uint8_t one_file_is_closed;

                read_index[i] = 0;
                one_file_is_closed = 1;
            }
           
            io_counter[i] += ios[i].period;
            threads |= 1 << (ios[i].instance_affinity);
        }
    }

    /* awake the thread of instance 0 */
    if (threads & 1)
    {
#ifdef RTOS_USED
        xTaskResumeFromISR(new_thread0);
#else
        extern void main_run(void);
        BareMetalTaskHandle |= BareMetalTaskHandle0_mask;
        main_run();
#endif
    }
    /* awake the thread of instance 1 */
    if (threads & 2)
    {
#ifdef RTOS_USED
        xTaskResumeFromISR(new_thread1);
#else
        BareMetalTaskHandle |= BareMetalTaskHandle1_mask;
#endif
    }
    /* awake the thread of instance 2 */
    if (threads & 4)
    {
#ifdef RTOS_USED
        xTaskResumeFromISR(new_thread2);
#else
        BareMetalTaskHandle |= BareMetalTaskHandle2_mask;
#endif
    }

}


#ifdef __cplusplus
}
#endif

