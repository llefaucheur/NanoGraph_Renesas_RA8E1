/* ----------------------------------------------------------------------
 * Project:      NanoGraph
 * Title:        platform_io_services.c
 * Description:  abstraction layer to BSP and streams from the application
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

#include <stdint.h>

#include "../nanograph_common.h"
#include "../nanograph_interpreter.h"
#include "../top_manifest_included.h"


#include "bsp_api.h"
#include "common_data.h"
FSP_HEADER
FSP_FOOTER


#ifdef __cplusplus
 extern "C" {
#endif

/*-----------------------------------------------------------------------*/
extern uint8_t one_file_is_closed;

extern void NanoGraph_io_ack (uint8_t HW_io_idx, void *data, uintptr_t size);

/*
 * NULL TASK
 */
extern void NanoGraph_null_task(int32_t c, nanograph_handle_t i, void* d, uint32_t* s);
void NanoGraph_null_task (int32_t c, nanograph_handle_t i, void *d, uint32_t *s)  {}


/*
    IO interfaces functions to the possible streams of the platform
 */
  extern void data_sink           (uint32_t, nanograph_xdmbuffer_t *);  //  0
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


/* --------------------------------------------------------------------------------------- 
    FW IO FUNCTIONS
*/


#define size_data_sink 16
static int16_t buffer_data_sink[size_data_sink / sizeof(int16_t)];

#define size_data_out_0 32
static int16_t buffer_data_out_0[size_data_out_0 / sizeof(int16_t)];        // 3    general purpose and links to the application ("general" domain)

#define size_sensor_in_0 32
static int16_t buffer_sensor_in_0[size_sensor_in_0 / sizeof(int16_t)];      // 5    motion, temperature, proximity, CO2, PIR ("motion" or "analog_in" or "gpio" domains)

#define size_timer_0 4
static int16_t buffer_timer_0[size_timer_0 / sizeof(int32_t)];              // 9    timer at 100ms, 1s ("timer" domain)

#define size_ui_in_0 4
static int16_t buffer_ui_in_0[size_ui_in_0 / sizeof(int32_t)];              // 11   buttons, slider ("user_interface" domain)

#define size_io_ui_out_0 96                                                 // 15   LED, LCD, Digits, Metadata ("user_interface" domain)
static uint32_t buffer_io_ui_out_0[size_io_ui_out_0 / sizeof(int32_t)];     

#define size_analog_in_0 96                                                 // 21   analog A/D input ("motion" or "analog_in" or "gpio" domain)
static uint32_t buffer_analog_in_0[size_analog_in_0 / sizeof(int32_t)];       

#define size_audio_in_0 96                                                  // 23   microphone, PDM, line-in, modem, USB audio ("audio_in" domain)
static uint32_t buffer_audio_in_0[size_audio_in_0 / sizeof(int32_t)];       

#define size_2d_in_0 96                                                     // 30   camera ("2d_in" domain)
static uint32_t buffer_2d_in_0[size_2d_in_0 / sizeof(int32_t)];     



/*
 * ---------------------IO_AL_idx = 0-----------------------------------
 */
void data_sink(uint32_t command, nanograph_xdmbuffer_t* data)
{   
}
/*
 * ---------------------IO_AL_idx = 1-----------------------------------
 */

void io_data_in_0(uint32_t command, nanograph_xdmbuffer_t* data)
{
    }

/*
 * ---------------------IO_AL_idx = 2-----------------------------------
 */

void io_data_in_1(uint32_t command, nanograph_xdmbuffer_t* data)
{
}



/*
 * ---------------------IO_AL_idx = 3-----------------------------------
 */

void io_data_out_0(uint32_t command, nanograph_xdmbuffer_t* data)
{
}

/*
 * ---------------------IO_AL_idx = 4-----------------------------------
 */

void io_data_out_1(uint32_t command, nanograph_xdmbuffer_t* data)
    {
}

/*
 * ---------------------IO_AL_idx = 5-----------------------------------
 */

void io_sensor_in_0 (uint32_t command, nanograph_xdmbuffer_t *data)             //  motion, temperature, proximity, CO2, PIR ("motion" or "analog_in" or "gpio" domains)
{   int32_t tmp, nanograph_format_io_setting;

#define IO_SENSOR_IN_0_SIZE 8
    int16_t local_buffer[IO_SENSOR_IN_0_SIZE];

    switch (command)
    {
    case NANOGRAPH_RESET:
        nanograph_format_io_setting = *(uint32_t *)(data->address);          
        break;
        
        case NANOGRAPH_SET_PARAMETER:
        break;
        case NANOGRAPH_SET_BUFFER:
        break;
        
        case NANOGRAPH_RUN:
        //NanoGraph_io_ack (IO_PLATFORM_SENSOR_IN_0, local_buffer, IO_SENSOR_IN_0_SIZE * sizeof(int16_t));
        break;

        case NANOGRAPH_STOP:
        one_file_is_closed = 1;
        break;
    default:
        break;      
    }
}


/*
 * ---------------------IO_AL_idx = 9-----------------------------------
 */
void io_timer_0 (uint32_t command, nanograph_xdmbuffer_t* data)              // timer at 100ms, 1s ("timer" domain)
    {
    }

/*
 * ---------------------IO_AL_idx = 11-----------------------------------
 */
void io_ui_in_0 (uint32_t command, nanograph_xdmbuffer_t* data)              // buttons, slider ("user_interface" domain)
{
}

/*
 * ---------------------IO_AL_idx = 15-----------------------------------
 */

void io_ui_out_0(uint32_t command, nanograph_xdmbuffer_t* data)             // LED, LCD, Digits, Metadata ("user_interface" domain)
{
    nanograph_xdmbuffer_t* pt_pt;

    switch (command)
    {
    case NANOGRAPH_RESET:
        break;
    case NANOGRAPH_SET_PARAMETER:
        break;
    case NANOGRAPH_SET_BUFFER:
    break;
    case NANOGRAPH_RUN:
    {	extern void hal_set_led0_low(void);
    	extern void hal_set_led0_high(void);

    	nanograph_xdmbuffer_t* pt_pt = (nanograph_xdmbuffer_t*)data;

        /* "io_platform_nanograph_in_1," frame_size option in samples + FORMAT-0 in the example graph */
        pt_pt = (nanograph_xdmbuffer_t*)data;
        NanoGraph_io_ack (IO_PLATFORM_UI_OUT_0, (uint8_t *)pt_pt->address, pt_pt->size);

        if (*(uint32_t*)pt_pt->address == 0)
        {   hal_set_led0_low();
        }
        else
        {	hal_set_led0_high();
        }
        break;
    }
    case NANOGRAPH_STOP:
        one_file_is_closed = 1;
        break;
    case NANOGRAPH_READ_PARAMETER: /* setting done ? device is ready ? calibrated ? */
        break;
    default:
        break;
    }
}

/*
 * ---------------------IO_AL_idx = 19-----------------------------------
 */
void io_serial_in_0(uint32_t command, nanograph_xdmbuffer_t* data)      // communication RX for debug and remote control ("general" domain)
{
}


/*
 * ---------------------IO_AL_idx = 21-----------------------------------
 */

void io_analog_in_0(uint32_t command, nanograph_xdmbuffer_t* data)      // analog A/D input ("motion" or "analog_in" or "gpio" domain)
{
}


/*
 * ---------------------IO_AL_idx = 23-----------------------------------
 */

void io_audio_in_0 (uint32_t command, nanograph_xdmbuffer_t *data)      // microphone, PDM, line-in, modem, USB audio ("audio_in" domain)
{   int32_t tmp, nanograph_format_io_setting;
    nanograph_xdmbuffer_t* pt_pt;

    switch (command)
    {
    case NANOGRAPH_RESET:
        nanograph_format_io_setting = *(uint32_t *)(data->address);
        break;
    case NANOGRAPH_SET_PARAMETER:
        break;
    case NANOGRAPH_SET_BUFFER:
        {   pt_pt = (nanograph_xdmbuffer_t *)data;
            pt_pt->address = (intptr_t)buffer_audio_in_0;
            pt_pt->size = size_audio_in_0;
    }
    break;
    case NANOGRAPH_RUN:
        pt_pt = (nanograph_xdmbuffer_t*)data;
        NanoGraph_io_ack (IO_PLATFORM_AUDIO_IN_0, buffer_audio_in_0, pt_pt->size);

        break;
    case NANOGRAPH_STOP:
        one_file_is_closed = 1;
        break;
    default:
        break;
    }
}


/*
 * ---------------------IO_AL_idx = 26-----------------------------------
 */

void io_audio_out_0(uint32_t command, nanograph_xdmbuffer_t* data)           // buzzer, class-D, earphone, line-out, ultrasound
{
}

/*
 * ---------------------IO_AL_idx = 30-----------------------------------
 */

void io_2d_in_0(uint32_t command, nanograph_xdmbuffer_t* data) 
{
}
