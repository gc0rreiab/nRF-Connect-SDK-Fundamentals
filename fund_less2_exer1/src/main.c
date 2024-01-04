/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS   100

//Get the device pointer, pin number and pin’s configuration flags 
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

void main(void)
{
	int ret;

	if (!device_is_ready(led.port)) {
		return;
	}

         if(!device_is_ready(button.port)) {
                return;
        }

        //GPIO_OUTPUT_ACTIVE = GPIO_OUTPUT | GPIO_OUTPUT_INIT_HIGH | GPIO_OUTPUT_INIT_LOGICAL
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

        ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
        if (ret < 0) {
                return;
        }

	while (1) {
                
                bool val = gpio_pin_get_dt(&button);

                gpio_pin_set_dt(&led, val);

		k_msleep(SLEEP_TIME_MS); // Put the main thread to sleep for 100ms for power optimization
	}
}