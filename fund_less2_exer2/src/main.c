/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

/* Define the sleep time to 10min */
#define SLEEP_TIME_MS   10*60*1000

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);


/* Define the callback function that will run when the button is pressed */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    gpio_pin_toggle_dt(&led);
}

/* Define a variable of type static struct gpio_callback */
static struct gpio_callback button_cb_data;


void main(void)
{
	int ret;

	if (!device_is_ready(led.port)) {
		return;
	}

	if (!device_is_ready(button.port)) {
		return;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return;
	}
	/* Configure the interrupt on the button's pin */
        ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);

	/* Initialize the static struct gpio_callback variable   */
        gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin)); 	

	/* Add the callback function by calling gpio_add_callback()   */
        gpio_add_callback(button.port, &button_cb_data);

	while (1) {
                k_msleep(SLEEP_TIME_MS); 
	}
}