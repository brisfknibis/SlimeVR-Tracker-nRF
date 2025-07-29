#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(imu_led, LOG_LEVEL_INF);

/* IMU LED pins (from your module pinout)
 * Red   = P0.02
 * Green = P1.15
 * Blue  = P0.29
 */
#define IMU_LED_RED_PIN     2   /* Port 0, pin 2  */
#define IMU_LED_GREEN_PIN   15  /* Port 1, pin 15 */
#define IMU_LED_BLUE_PIN    29  /* Port 0, pin 29 */

/* If your RGB is common-anode, set to 0 to drive low = ON */
#ifndef IMU_LED_ACTIVE_HIGH
#define IMU_LED_ACTIVE_HIGH 0
#endif

/* Resolve GPIO ports at build time from devicetree labels */
#define GPIO0_NODE DT_NODELABEL(gpio0)
#define GPIO1_NODE DT_NODELABEL(gpio1)

static const struct device *const gpio0 = DEVICE_DT_GET(GPIO0_NODE);
static const struct device *const gpio1 = DEVICE_DT_GET(GPIO1_NODE);

static inline void imu_led_set_rgb(bool r, bool g, bool b)
{
    const int on  = IMU_LED_ACTIVE_HIGH ? 1 : 0;
    const int off = IMU_LED_ACTIVE_HIGH ? 0 : 1;

    gpio_pin_set(gpio0, IMU_LED_RED_PIN,   r ? on : off);
    gpio_pin_set(gpio1, IMU_LED_GREEN_PIN, g ? on : off);
    gpio_pin_set(gpio0, IMU_LED_BLUE_PIN,  b ? on : off);
}

/* NOTE: No-arg signature to match your Zephyr SYS_INIT expectations */
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

    /* Solid white: R+G+B ON */
    imu_led_set_rgb(true, true, true);
    LOG_INF("IMU LED set to solid white");
    return 0;
}

SYS_INIT(imu_led_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
