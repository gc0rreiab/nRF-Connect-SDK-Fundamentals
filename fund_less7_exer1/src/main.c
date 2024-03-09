/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// Macro to build the exercise with k_yield() or with k_msleep()
// #define YIELD_EXAMPLE

/* Define stack size and scheduling priority used by each thread */
#define STACKSIZE 1024 // 1KB stack size
#define THREAD0_PRIORITY 7
#define THREAD1_PRIORITY 7

void thread0(void)
{
    while (1)
    {
        printk("Hello, I am thread0\n");
#ifdef YIELD_EXAMPLE
        /* Make the thread yield */
        k_yield();
#else
        /* Put the thread to sleep */
        k_msleep(5);
#endif
    }
}

void thread1(void)
{
    while (1)
    {
        printk("Hello, I am thread1\n");
#ifdef YIELD_EXAMPLE
        /* Make the thread yield */
        k_yield();
#else
        /* Put the thread to sleep */
        k_msleep(5);
#endif
    }
}

/* Define and initialize the two threads */
K_THREAD_DEFINE(thread0_id, STACKSIZE, thread0, NULL, NULL, NULL,
                THREAD0_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACKSIZE, thread1, NULL, NULL, NULL,
                THREAD1_PRIORITY, 0, 0);

int main(void)
{
}