#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(imu_led, LOG_LEVEL_INF);

/* IMU LED pins (from your module pinout)
 * Red   = P0.02
 * Green = P1.15
 * Blue  = P0.29
 */
#define IMU_LED_RED_PORT    "GPIO_0"
#define IMU_LED_RED_PIN     2
#define IMU_LED_GREEN_PORT  "GPIO_1"
#define IMU_LED_GREEN_PIN   15
#define IMU_LED_BLUE_PORT   "GPIO_0"
#define IMU_LED_BLUE_PIN    29

static const struct device *gpio0;
static const struct device *gpio1;

static int imu_led_init(const struct device *dev)
{
    ARG_UNUSED(dev);

    gpio0 = device_get_binding(IMU_LED_RED_PORT);   // Port 0 (red + blue)
    gpio1 = device_get_binding(IMU_LED_GREEN_PORT); // Port 1 (green)

    if (!gpio0 || !gpio1) {
        LOG_ERR("GPIO_0/1 device binding failed");
        return -ENODEV;
    }

    /* Configure as outputs (start low), then set high to turn ON.
       If your LED wiring is inverted (common-anode), swap 1 -> 0 below. */
    int err = 0;
    err |= gpio_pin_configure(gpio0, IMU_LED_RED_PIN,   GPIO_OUTPUT);
    err |= gpio_pin_configure(gpio1, IMU_LED_GREEN_PIN, GPIO_OUTPUT);
    err |= gpio_pin_configure(gpio0, IMU_LED_BLUE_PIN,  GPIO_OUTPUT);
    if (err) {
        LOG_ERR("gpio_pin_configure failed (%d)", err);
        return err;
    }

    /* Solid white: turn on R + G + B */
    gpio_pin_set(gpio0, IMU_LED_RED_PIN,   1);
    gpio_pin_set(gpio1, IMU_LED_GREEN_PIN, 1);
    gpio_pin_set(gpio0, IMU_LED_BLUE_PIN,  1);

    LOG_INF("IMU LED forced to solid white");
    return 0;
}

/* Run at app init */
SYS_INIT(imu_led_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
