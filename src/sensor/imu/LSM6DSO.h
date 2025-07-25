#ifndef LSM6DSO_h
#define LSM6DSO_h

#include "sensor/sensor.h"

// https://www.st.com/resource/en/datasheet/lsm6dso.pdf
#define LSM6DSO_FIFO_CTRL3                 0x09
#define LSM6DSO_FIFO_CTRL4                 0x0A

#define LSM6DSO_CTRL1                      0x10
#define LSM6DSO_CTRL2                      0x11
#define LSM6DSO_CTRL3                      0x12
#define LSM6DSO_CTRL4                      0x13
#define LSM6DSO_CTRL5                      0x14
#define LSM6DSO_CTRL6                      0x15
#define LSM6DSO_CTRL7                      0x16
#define LSM6DSO_CTRL8                      0x17

#define LSM6DSO_FIFO_STATUS1               0x3A

#define LSM6DSO_TAP_CFG0                   0x56
#define LSM6DSO_TAP_CFG2                   0x58
#define LSM6DSO_WAKE_UP_THS                0x5B
#define LSM6DSO_MD1_CFG                    0x5E

#define LSM6DSO_INTERNAL_FREQ_FINE         0x63

#define LSM6DSO_FIFO_DATA_OUT_TAG          0x78

#define DSO_FS_G_250DPS  0x00 //0bxxxx0000
#define DSO_FS_G_500DPS  0x04 //0bxxxx0100
#define DSO_FS_G_1000DPS 0x08 //0bxxxx1000
#define DSO_FS_G_2000DPS 0x0C //0bxxxx1100

#define DSO_FS_XL_2G  0x00 // 0bxxxx0000
#define DSO_FS_XL_4G  0x08 // 0bxxxx1000
#define DSO_FS_XL_8G  0x0C // 0bxxxx1100
#define DSO_FS_XL_16G 0x04 // 0bxxxx0100

// Same for XL and G
#define DSO_ODR_OFF     0b00000000
#define DSO_ODR_12_5Hz  0b00010000
#define DSO_ODR_26Hz    0b00100000
#define DSO_ODR_52Hz    0b00110000
#define DSO_ODR_104Hz   0b01000000
#define DSO_ODR_208Hz   0b01010000
#define DSO_ODR_416Hz   0b01100000
#define DSO_ODR_833Hz   0b01110000
#define DSO_ODR_1_66kHz 0b10000000
#define DSO_ODR_3_33kHz 0b10010000
#define DSO_ODR_6_66kHz 0b10100000

#define DSO_OP_MODE_XL_HP    0x00 // High Performance
#define DSO_OP_MODE_XL_NP    0x10 // Low Power

#define DSO_OP_MODE_G_HP    0x00 // High Performance
#define DSO_OP_MODE_G_NP    0x80 // Low Power

#define DSO_OP_MODE_G_AWAKE 0x00 // Gyro active
#define DSO_OP_MODE_G_SLEEP 0x40 // Gyro sleep

int lsm6dso_init(float clock_rate, float accel_time, float gyro_time, float *accel_actual_time, float *gyro_actual_time);

void lsm6dso_update_fs(float accel_range, float gyro_range, float *accel_actual_range, float *gyro_actual_range);
int lsm6dso_update_odr(float accel_time, float gyro_time, float *accel_actual_time, float *gyro_actual_time);

uint16_t lsm6dso_fifo_read(uint8_t *data, uint16_t len);

uint8_t lsm6dso_setup_WOM(void);

extern const sensor_imu_t sensor_imu_lsm6dso;

#endif
