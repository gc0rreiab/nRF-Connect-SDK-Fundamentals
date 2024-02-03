/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

/* Defining all MACROS related to uart reception buffer */
#define RECEIVE_BUFF_SIZE 10
#define RECEIVE_TIMEOUT 100

/* Get the device pointer of the leds hardware */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

/* Get the device pointer of the UART0 hardware. */
const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

/* Defining the transmission buffer */
static uint8_t tx_buf[] = {"nRF Connect SDK Fundamentals Course\n\r"
						   "Press 1-3 on your keyboard to toggle LEDS 1-3 on your development kit\n\r"};

/* Defining the reception buffer */
static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};

/* Defining declaration of callback functions */
static void uart_cb(const struct device *, struct uart_event *, void *);

int main(void)
{
	int ret;

	/* Since all the LEDs are associated with the same port, &gpio0, we only need to check one of the devices. */
	if (!device_is_ready(led0.port))
	{
		printk("GPIO device is not ready\r\n");
		return 0;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 1;
	}
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 1;
	}
	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 1;
	}

	/* Check if uart0 device is ready*/
	if (!device_is_ready(uart))
	{
		printk("UART0 device not ready\r\n");
		return 1;
	}

	/* Register the uart0 callback function */
	ret = uart_callback_set(uart, uart_cb, NULL);
	if (ret)
	{
		return 1;
	}

	/* Send some data over uart0 peripheral */
	ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
	if (ret)
	{
		return 1;
	}

	/* Enabling the uart0 peripheral reception */
	ret = uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
	if (ret)
	{
		return 1;
	}

	while (1)
	{
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}

/* Defining callbacks */
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type)
	{
	case UART_RX_RDY:

		if ((evt->data.rx.len) == 1)
		{
			if (evt->data.rx.buf[evt->data.rx.offset] == '1')
				gpio_pin_toggle_dt(&led0);
			else if (evt->data.rx.buf[evt->data.rx.offset] == '2')
				gpio_pin_toggle_dt(&led1);
			else if (evt->data.rx.buf[evt->data.rx.offset] == '3')
				gpio_pin_toggle_dt(&led2);
		}

		break;
	case UART_RX_DISABLED:
		uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
		break;
	default:
		break;
	}
}