#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "led.h"

LOG_MODULE_REGISTER(imu_led, LOG_LEVEL_INF);

/* IMU LED pins (from your module pinout)
 * Red   = P0.02
 * Green = P1.15
 * Blue  = P0.29
 */
#define IMU_LED_RED_PIN     2    /* Port 0, pin 2  */
#define IMU_LED_GREEN_PIN   15   /* Port 1, pin 15 */
#define IMU_LED_BLUE_PIN    29   /* Port 0, pin 29 */

/* Common-anode LED: drive LOW to turn ON */
#define IMU_LED_ACTIVE_HIGH 0

/* Resolve GPIO ports at build time from devicetree labels */
#define GPIO0_NODE DT_NODELABEL(gpio0)
#define GPIO1_NODE DT_NODELABEL(gpio1)

static const struct device *const gpio0 = DEVICE_DT_GET(GPIO0_NODE);
static const struct device *const gpio1 = DEVICE_DT_GET(GPIO1_NODE);

static bool imu_led_ready = false;

/* Internal helper: set raw RGB booleans (true = ON logically) */
static inline void set_rgb_raw(bool r, bool g, bool b)
{
    const int on  = IMU_LED_ACTIVE_HIGH ? 1 : 0;  /* 0 for common-anode */
    const int off = IMU_LED_ACTIVE_HIGH ? 0 : 1;  /* 1 for common-anode */

    gpio_pin_set(gpio0, IMU_LED_RED_PIN,   r ? on : off);
    gpio_pin_set(gpio1, IMU_LED_GREEN_PIN, g ? on : off);
    gpio_pin_set(gpio0, IMU_LED_BLUE_PIN,  b ? on : off);
}

/* Exported: force IMU LED off */
void imu_led_off(void)
{
    if (!imu_led_ready) return;
    set_rgb_raw(false, false, false);
}

void imu_led_sync(enum sys_led_color color, int value_pptt)
{
    if (!imu_led_ready) return;

    /* Treat >50% as ON since we’re using plain GPIO (no PWM here) */
    const bool on = (value_pptt > 5000);

    /* Keep this in sync with your enum values in led.h */
    switch (color) {
    case 0: /* SYS_LED_COLOR_DEFAULT -> Blue */
        set_rgb_raw(false, false, on);
        break;
    case 1: /* SYS_LED_COLOR_SUCCESS -> Green */
        set_rgb_raw(false, on, false);
        break;
    case 2: /* SYS_LED_COLOR_ERROR -> Red */
        set_rgb_raw(on, false, false);
        break;
    case 3: /* SYS_LED_COLOR_CHARGING -> Yellow (Red + Green) */
        set_rgb_raw(on, on, false);
        break;
    default:
        set_rgb_raw(false, false, false);
        break;
    }
}

/* No-arg SYS_INIT for your Zephyr version */
static int imu_led_init(void)
{
    if (!device_is_ready(gpio0) || !device_is_ready(gpio1)) {
        LOG_ERR("GPIO controller(s) not ready");
        return -ENODEV;
    }

    int err = 0;
    err |= gpio_pin_configure(gpio0, IMU_LED_RED_PIN,   GPIO_OUTPUT);
    err |= gpio_pin_configure(gpio1, IMU_LED_GREEN_PIN, GPIO_OUTPUT);
    err |= gpio_pin_configure(gpio0, IMU_LED_BLUE_PIN,  GPIO_OUTPUT);
    if (err) {
        LOG_ERR("gpio_pin_configure failed (%d)", err);
        return err;
    }

    /* Quick sanity flash on boot */
    set_rgb_raw(true, true, true);
    k_msleep(120);
    set_rgb_raw(false, false, false);

    imu_led_ready = true;
    LOG_INF("IMU LED ready (common-anode / active-low)");
    return 0;
}

SYS_INIT(imu_led_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
