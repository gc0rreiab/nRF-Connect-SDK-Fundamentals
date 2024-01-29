/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* Define the sleep time to 10min */
#define SLEEP_TIME_MS 10 * 60 * 1000
/* Macro that represents the maximum number to calculate its factorial */
#define MAX_NUMBER_FACT 10

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

/* Define the callback function that will run when the button is pressed */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	int i;
	int j;
	long int factorial;
	printk("Calculating the factorials of numbers from 1 to %d:\n\r", MAX_NUMBER_FACT);
	for (i = 1; i <= MAX_NUMBER_FACT; i++)
	{
		factorial = 1;
		for (j = 1; j <= i; j++)
		{
			factorial = factorial * j;
		}
		printk("The factorial of %2d = %ld \n\r", i, factorial);
	}
	printk("_______________________________________________________\n\r");
	/*Important note!
	Code in ISR runs at a high priority, therefore, it should be written with timing in mind.
	Too lengthy or too complex tasks should not be performed by an ISR, they should be deferred to a thread.
	*/
	gpio_pin_toggle_dt(&led);
}

/* Define a variable of type static struct gpio_callback */
static struct gpio_callback button_cb_data;

void main(void)
{
	int ret;

	printk("nRF Connect SDK Fundamentals - Lesson 4 - Exercise 1\n\r");

	if (!device_is_ready(led.port))
	{
		return;
	}

	if (!device_is_ready(button.port))
	{
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0)
	{
		return;
	}
	/* Configure the interrupt on the button's pin */
	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

	/* Initialize the static struct gpio_callback variable   */
	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));

	/* Add the callback function by calling gpio_add_callback()   */
	gpio_add_callback(button.port, &button_cb_data);

	while (1)
	{
		k_msleep(SLEEP_TIME_MS);
	}
}