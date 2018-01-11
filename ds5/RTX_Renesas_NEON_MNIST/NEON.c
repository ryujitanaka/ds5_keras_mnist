/*----------------------------------------------------------------------------
 *      CMSIS-RTOS  -  RTX
 *----------------------------------------------------------------------------
 *      Name:    NEON.C
 *      Purpose: RTX example program
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2015 ARM Germany GmbH
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

#include <stdio.h>                    /* standard I/O .h-file                */
#include "rt_TypeDef.h"
#include "rt_Time.h"
#include "cmsis_os.h"
#include "cnn.h"
#include "barman.h"

extern const bm_uint8 Image$$BARMAN_BUFFER$$ZI$$Base;
extern const bm_uint8 Image$$BARMAN_BUFFER$$ZI$$Limit;

#define BARMAN_BUFFER_LENGTH    (&Image$$BARMAN_BUFFER$$ZI$$Limit - &Image$$BARMAN_BUFFER$$ZI$$Base)
#define BARMAN_BUFFER           ((bm_uint8*) &Image$$BARMAN_BUFFER$$ZI$$Base)

/*
 * Perform the necessary initialization of the bare-metal agent
 */
static void enable_barman(void)
{
   /* RTX timer operates at 1KHz frequency, so configure clock timer multiplier as 1000000 and divisor as 1 */
    const struct bm_protocol_clock_info clock_info = { .timestamp_base = 0,
                                                       .timestamp_multiplier = 1000000,
                                                       .timestamp_divisor = 1,
                                                       .unix_base_ns = 0 };

#if BM_CONFIG_MAX_TASK_INFOS > 0
    const struct bm_protocol_task_info task_entries[] =
    {
        { 0, "start" },
        { 1, "osTimerThread" },
        { 2, "main-or-get_escape" },
        { 3, "lcd" },
        { 4, "command" },
        { 5, "lights" },
        { 6, "keyread" },
        { 255, "os_idle_demon" },
    };
#endif

    /* Initialize barman but if there is a problem we will loop here */
    while (!barman_initialize(BARMAN_BUFFER, BARMAN_BUFFER_LENGTH, "RTXv4 Streamline bare-metal example", &clock_info,
#if BM_CONFIG_MAX_TASK_INFOS > 0
                              /* All the tasks, excluding idle (= 255) */
                              8, task_entries,
#endif
#if BM_CONFIG_MAX_MMAP_LAYOUTS > 0
                              /* We only have one image for all tasks so we don't need to provide these */
                              0, BM_NULL,
#endif
                              1));

    /* Now we are ready to enable sampling */
    barman_enable_sampling();
}

/* Monotonically increasing timestamp_counter, incremented on every RTX timer tick */
static bm_uint64 timestamp_counter = 0;

void increment_timestamp_counter(void)
{
    timestamp_counter += 1;
}

bm_uint64 barman_ext_get_timestamp(void)
{
    return timestamp_counter;
}

/* Allow barman to read the current task id from RTX */
extern bm_task_id_t rt_tsk_self (void);

bm_task_id_t barman_ext_get_current_task_id(void)
{
    return rt_tsk_self();
}


/*----------------------------------------------------------------------------
 *   Main Thread
 *---------------------------------------------------------------------------*/
int main (void) {                     /* program execution starts here       */
	unsigned int inference;
	unsigned int starttime;
	unsigned int endtime;

	enable_barman();					/* enable barman */

	starttime = rt_time_get();	// OS_TICK defined as 1000(1ms) on RTX_Conf_CM.c

	barman_annotate_marker(BM_ANNOTATE_COLOR_GREEN, "mnist_cnn_eval:START");
	mnist_cnn_eval((unsigned int *)TESTDATA, &inference);
	barman_annotate_marker(BM_ANNOTATE_COLOR_GREEN, "mnist_cnn_eval:END");

	endtime = rt_time_get();

	printf("MNIST: %d (%d ms)\n", inference, endtime - starttime);

	return 0;
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

