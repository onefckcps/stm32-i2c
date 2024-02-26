#pragma once

#include <cstdio>
#include <cinttypes>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "i2cdev.h"

#define VEML3328_I2C_ADDR 0x10

#define VEML3328_DEVICE_ID 0x28

// Registers (Command codes)
#define VEML3328_REG_CONFIG 0x00
#define VEML3328_REG_CLEAR 0x04
#define VEML3328_REG_RED 0x05
#define VEML3328_REG_GREEN 0x06
#define VEML3328_REG_BLUE 0x07
#define VEML3328_REG_IR 0x08
#define VEML3328_REG_ID 0x0C

// Bit offsets
#define VEML3328_OFFSET_SD1 15
#define VEML3328_OFFSET_SD_ALS 14
#define VEML3328_OFFSET_DG 12
#define VEML3328_OFFSET_GAIN 10
#define VEML3328_OFFSET_SENS 6
#define VEML3328_OFFSET_IT 4
#define VEML3328_OFFSET_AF 3
#define VEML3328_OFFSET_TRIG 2
#define VEML3328_OFFSET_SD0 0

#define VEML3328_MAX_SENSITIVITY 3 // 3 mLux/step (DG 4, gain 4, IT 400ms)

typedef uint16_t veml3328_dg_t;
#define VEML3328_DG_1 0x0
#define VEML3328_DG_2 0x1
#define VEML3328_DG_4 0x2
#define VEML3328_CONFIG_DG_MASK (0x3 << VEML3328_OFFSET_DG)

typedef uint8_t veml3328_gain_t;
#define VEML3328_GAIN_HALF 0x3
#define VEML3328_GAIN_1 0x0
#define VEML3328_GAIN_2 0x1
#define VEML3328_GAIN_4 0x2
#define VEML3328_CONFIG_GAIN_MASK (0x3 << VEML3328_OFFSET_GAIN)

typedef uint8_t veml3328_it_t;
#define VEML3328_IT_50 0x0
#define VEML3328_IT_100 0x1
#define VEML3328_IT_200 0x2
#define VEML3328_IT_400 0x3
#define VEML3328_CONFIG_IT_MASK (0x3 << VEML3328_OFFSET_IT)

typedef uint8_t veml3328_sens_t;
#define VEML3328_SENS_HIGH 0x0
#define VEML3328_SENS_LOW 0x1
#define VEML3328_CONFIG_SENS_MASK (0x1 << VEML3328_OFFSET_SENS)

#define VEML3328_CONFIG_SENSITIVITY_MASK (VEML3328_CONFIG_DG_MASK | VEML3328_CONFIG_GAIN_MASK | VEML3328_CONFIG_SENS_MASK | VEML3328_CONFIG_IT_MASK)

class VEML3328 final
{
private:
  I2C *i2c_dev;
  static constexpr char *TAG = (char *)"VEML3328";
  static constexpr uint16_t available_sensitivity[] = {3, 6, 12, 24, 48, 96, 192, 384, 768, 1152, 2304};
  uint16_t config;
  uint16_t conversion_factor;
  uint8_t current_sensitivity_index;
  uint16_t red;
  uint32_t red_lux;
  uint16_t green;
  uint32_t green_lux;
  uint16_t blue;
  uint32_t blue_lux;
  uint16_t clear;
  uint32_t clear_lux;
  uint16_t ir;
  uint32_t ir_lux;

public:
  VEML3328(I2C *dev);
  esp_err_t check_device_id();
  uint8_t get_it_conversion_factor();
  uint8_t get_dg_conversion_factor();
  uint8_t get_gain_conversion_factor();
  uint8_t get_sens_conversion_factor();

  uint16_t calc_conversion_factor();
  esp_err_t read_config();
  esp_err_t write_config();
  esp_err_t read_data();
  esp_err_t set_sensitivity(int8_t sensitivity_index);
  esp_err_t start_auto();
  uint32_t get_red_in_lux();
  uint32_t get_green_in_lux();
  uint32_t get_blue_in_lux();
  uint32_t get_clear_in_lux();
  uint32_t get_ir_in_lux();
  esp_err_t increase_sensitivity();
  esp_err_t decrease_sensitivity();
};