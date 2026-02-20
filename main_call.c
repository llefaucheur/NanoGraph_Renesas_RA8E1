/* ----------------------------------------------------------------------
 * Project:      CMSIS Stream
 * Title:        main.c
 * Description:  graph interpreter demo
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


#include "top_manifest_included.h"
#include "nanograph_common.h"
#include "nanograph_interpreter.h"



/*
    global variables : all the instances of the graph interpreter
*/
#define NANOGRAPH_CURRENT_INSTANCE 0

extern nanograph_instance_t* platform_io_callback_parameter;
extern uint8_t platform_io_instance_idx;

uintptr_t all_ptr_instances[NANOGRAPH_NB_INSTANCE];
nanograph_instance_t my_instance;


/**
  @brief        check all instance running on the platform
  @param[in]    none
  @return       set RSTSTATE_DONE_SYNC in each instance when reset is done

  @par
  @remark
 */
void check_instance_left_reset(void);
void check_instance_left_reset(void)
{
    uint8_t i, all_reset_done, R;
    nanograph_instance_t *instance;

    all_reset_done = 1;

    /* check all instance have completed the RESET stage */
    for (i = 0; i < NANOGRAPH_NB_INSTANCE; i++)
    {
        instance = (nanograph_instance_t*)(all_ptr_instances[i]);
        R = RD(instance->scheduler_control, RSTSTATE_SCTRL);
        all_reset_done = all_reset_done & (R == RSTSTATE_DONE);
    }

    /* if YES, then change the state of all the instances : RUN can be executed */
    if (all_reset_done)
    {   for (i = 0; i < NANOGRAPH_NB_INSTANCE; i++)
        {
        instance = (nanograph_instance_t*)(all_ptr_instances[i]);
        ST(instance->scheduler_control, RSTSTATE_SCTRL, RSTSTATE_DONE_SYNC);
        }
    }
}



/**
  @brief            (main) demonstration
  @param[in/out]    none
  @return           int
  @remark
 */
void main_init(uint32_t *graph);
void main_init(uint32_t *graph)
{ 
    my_instance.scheduler_control = PACK_NANOGRAPH_PARAM(
            NANOGRAPH_CURRENT_INSTANCE,            // instance index
            NANOGRAPH_INSTANCE_LOWLATENCYTASKS,    // low-latency priority
            GLOBAL_MAIN_INSTANCE,               // this interpreter instance is the main one (multi-thread)
            COMMDEXT_COLD_BOOT,                 // is it a warm or cold boot
            NANOGRAPH_SCHD_NO_SCRIPT,              // debugging scheme used during execution
            NANOGRAPH_SCHD_RET_END_ALL_PARSED      // interpreter returns after all nodes are parsed
            );

    /* provision protocol for situation when the graph comes from the application */
    my_instance.graph = graph;

    /*  the Graph interpreter instance holds the memory pointers
        this is a read-only global variable to let the IO have access to ongoing[], arcs[], formats[] ..*/
    platform_io_instance_idx = NANOGRAPH_CURRENT_INSTANCE;
    all_ptr_instances[NANOGRAPH_CURRENT_INSTANCE] = (uint64_t)(uintptr_t)(&my_instance);
    platform_io_callback_parameter = &my_instance;

    /* reset the graph */
    nanograph_interpreter (NANOGRAPH_RESET, &my_instance, 0, 0);
}


/**
  @brief            (main) demonstration
  @param[in/out]    none
  @return           int
  @remark
 */
void main_set_parameters(void);
void main_set_parameters(void)
{ 
//    uint32_t new_parameters_arm_stream_router__0[4] = { 1,2,3,4 };
//
//#define arm_stream_router__0       0x26 // node position in the graph
//    nano_graph_interpreter (STREAM_SET_PARAMETER, &(instance[STREAM_CURRENT_INSTANCE]),
//        arm_stream_router__0, (uintptr_t)new_parameters_arm_stream_router__0);

/*
    STREAM_SET_PARAMETER from scripts :
    1) main script calls nano_graph_interpreter (STREAM_SET_PARAMETER, IDX, @param)
    2) the script associated to the node calls S->application_callbacks[USE_CASE_CONTROL]
        then updates its node
*/
}


/**
  @brief            (main) demonstration
  @param[in/out]    none
  @return           int
  @remark
 */
void main_run(void);
void main_run(void)
{
    /* here test the need for memory recovery/swap 
        does the application modified the memory banks used by the graph ? */
    if (0)
    {   //arm_memory_swap(&(instance[STREAM_CURRENT_INSTANCE]));
    }

    nanograph_interpreter (NANOGRAPH_RUN, &my_instance, 0, 0);

    /* here test the need for memory recovery/swap
        does the application intend to modify memory banks used by the graph ? */
    if (0)
    {   //arm_memory_swap(&(instance[STREAM_CURRENT_INSTANCE]));
    }

}
/**
  @brief            Example of Master IO pushing new data
  @param[in/out]    none
  @return           int
  @remark
 */

void Push_Ping_Pong(uint32_t *data, uint32_t size);
void Push_Ping_Pong(uint32_t *data, uint32_t size)
{
    extern void NanoGraph_io_ack (uint8_t graph_io_idx, void *data, uintptr_t size);

    NanoGraph_io_ack (IO_PLATFORM_SENSOR_0, (uint8_t *)data, size);
}


/**
  @brief            (main) demonstration
  @param[in/out]    none
  @return           int
  @remark
 */
void main_stop(void);
void main_stop(void)
{
    nanograph_interpreter (NANOGRAPH_STOP, &my_instance, 0, 0);
}
