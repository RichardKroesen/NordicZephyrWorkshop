/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static struct k_work_delayable blink_led_work; // Work item for blinking the LED

static void blink_led(struct k_work *work)
{
	int ret;
	ret = gpio_pin_toggle_dt(&led);
	if (ret < 0) {
		return;
	}

	k_work_schedule(&blink_led_work, K_MSEC(CONFIG_BLINKY_TIME_ON));
}

int main(void)
{
	int ret;
	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	k_work_init_delayable(&blink_led_work, blink_led);
	k_work_schedule(&blink_led_work, K_NO_WAIT);

	return 0;
}
