#include <cstdio>
#include <cinttypes>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "i2cdev.h"

#include "VEML3328.h"

VEML3328::VEML3328(I2C *dev) {
  i2c_dev = dev;

  ESP_ERROR_CHECK(check_device_id());
  ESP_ERROR_CHECK(read_config());
}

esp_err_t VEML3328::check_device_id() {
  uint16_t id;
  esp_err_t err = i2c_dev->read_from_device_register(VEML3328_I2C_ADDR, VEML3328_REG_ID, &id);

  if (err != ESP_OK) {
    return err;
  }

  if ((uint8_t) id != VEML3328_DEVICE_ID) {
    ESP_LOGE(TAG, "Invalid device ID: 0x%04" PRIx16, id);
    return ESP_ERR_INVALID_RESPONSE;
  }

  return ESP_OK;
}

uint8_t VEML3328::get_it_conversion_factor() {
  uint8_t it = (config & (0b11 << VEML3328_OFFSET_IT)) >> VEML3328_OFFSET_IT;
  switch (it) {
    case VEML3328_IT_50:
      return 8;
    case VEML3328_IT_100:
      return 4;
    case VEML3328_IT_200:
      return 2;
    case VEML3328_IT_400:
      return 1;
  }

  //should never be reached
  return 0;
}

uint8_t VEML3328::get_dg_conversion_factor() {
  uint8_t dg = (config & (0b11 << VEML3328_OFFSET_DG)) >> VEML3328_OFFSET_DG;
  switch (dg) {
    case VEML3328_DG_1:
      return 4;
    case VEML3328_DG_2:
      return 2;
    case VEML3328_DG_4:
      return 1;
  }

  ESP_LOGE(TAG, "DG not supported! DG=0x%02" PRIx8 "\n", dg);
  return 0;
}

uint8_t VEML3328::get_gain_conversion_factor() {
  uint8_t gain = (config & (0b11 << VEML3328_OFFSET_GAIN)) >> VEML3328_OFFSET_GAIN;
  switch (gain) {
    case VEML3328_GAIN_HALF:
      return 8;
    case VEML3328_GAIN_1:
      return 4;
    case VEML3328_GAIN_2:
      return 2;
    case VEML3328_GAIN_4:
      return 1;
  }

  //should never be reached
  return 0;
}

uint8_t VEML3328::get_sens_conversion_factor() {
  uint8_t sens = (config & (0b1 << VEML3328_OFFSET_SENS)) >> VEML3328_OFFSET_SENS;
  if (sens) {
    return 3;
  }

  return 1;
}


uint16_t VEML3328::calc_conversion_factor() {
  uint16_t conversion_factor_tmp = get_it_conversion_factor() * \
                              get_dg_conversion_factor() * \
                              get_gain_conversion_factor() * \
                              get_sens_conversion_factor() * VEML3328_MAX_SENSITIVITY;
  
  uint8_t hit = 255;
  for(uint8_t i = 0; i < 11; i++) {
    if(available_sensitivity[i] == conversion_factor_tmp) {
      hit = i;
      break;
    }
  }

  if(hit > 10) {
    ESP_LOGW(TAG, "used non standard sensitivity config");
  }

  conversion_factor = conversion_factor_tmp;

  return conversion_factor_tmp;

}

esp_err_t VEML3328::read_config() {
  esp_err_t err = i2c_dev->read_from_device_register(VEML3328_I2C_ADDR, VEML3328_REG_CONFIG, &config);
  
  calc_conversion_factor();

  return err;
}

esp_err_t VEML3328::write_config() {
  esp_err_t err = i2c_dev->write_to_device_register(VEML3328_I2C_ADDR, VEML3328_REG_CONFIG, config);

  //printf("config written: 0x%04"PRIx16"\n", config);

  calc_conversion_factor();
  return err;
}

esp_err_t VEML3328::read_data() {
  esp_err_t err = i2c_dev->read_from_device_register(VEML3328_I2C_ADDR, VEML3328_REG_RED, &red);
  err |= i2c_dev->read_from_device_register(VEML3328_I2C_ADDR, VEML3328_REG_GREEN, &green);
  err |= i2c_dev->read_from_device_register(VEML3328_I2C_ADDR, VEML3328_REG_BLUE, &blue);
  err |= i2c_dev->read_from_device_register(VEML3328_I2C_ADDR, VEML3328_REG_CLEAR, &clear);
  err |= i2c_dev->read_from_device_register(VEML3328_I2C_ADDR, VEML3328_REG_IR, &ir);

  //read_config();
  //printf("red: 0x%04"PRIx16"\n", red);
  //printf("config: 0x%04"PRIx16"\n", config);

  red_lux = red * conversion_factor;
  green_lux = green * conversion_factor;
  blue_lux = blue * conversion_factor;
  clear_lux = clear * conversion_factor;
  ir_lux = ir * conversion_factor;

  if(green > 30000) {
    decrease_sensitivity();
    ESP_LOGI(TAG, "decreased sensitivity");
    vTaskDelay(pdMS_TO_TICKS(200));
  }

  if(green < 2000) {
    increase_sensitivity();
    ESP_LOGI(TAG, "increased sensitivity");
    vTaskDelay(pdMS_TO_TICKS(200));
  }

  if(err != ESP_OK) {
    ESP_LOGE(TAG, "Could not read data from device");
    return ESP_FAIL;
  }

  return ESP_OK;
}

esp_err_t VEML3328::set_sensitivity(int8_t sensitivity_index) {
  if (sensitivity_index > 10) {
    ESP_LOGI(TAG, "Can not decrease sensitivity, already at min");
    return set_sensitivity(10);
  }

  if (sensitivity_index < 0) {
    ESP_LOGI(TAG, "Can not increase sensitivity, already at max");
    return set_sensitivity(0);
  }

  uint16_t config_tmp = 0x0000;

  switch (sensitivity_index) {
    case 0:
      config_tmp = (VEML3328_DG_4 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_4 << VEML3328_OFFSET_GAIN) | (VEML3328_IT_400 << VEML3328_OFFSET_IT);
      break;
    case 1:
      config_tmp = (VEML3328_DG_4 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_4 << VEML3328_OFFSET_GAIN) | (VEML3328_IT_200 << VEML3328_OFFSET_IT);
      break;
    case 2:
      config_tmp = (VEML3328_DG_4 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_4 << VEML3328_OFFSET_GAIN) | (VEML3328_IT_100 << VEML3328_OFFSET_IT);
      break;
    case 3:
      config_tmp = (VEML3328_DG_4 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_4 << VEML3328_OFFSET_GAIN) | (VEML3328_IT_50 << VEML3328_OFFSET_IT);
      break;
    case 4:
      config_tmp = (VEML3328_DG_4 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_2 << VEML3328_OFFSET_GAIN) | (VEML3328_IT_50 << VEML3328_OFFSET_IT);
      break;
    case 5:
      config_tmp = (VEML3328_DG_4 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_1 << VEML3328_OFFSET_GAIN) | (VEML3328_IT_50 << VEML3328_OFFSET_IT);
      break;
    case 6:
      config_tmp = (VEML3328_DG_2 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_1 << VEML3328_OFFSET_GAIN) | (VEML3328_IT_50 << VEML3328_OFFSET_IT);
      break;
    case 7:
      config_tmp = (VEML3328_DG_1 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_1 << VEML3328_OFFSET_GAIN) | (VEML3328_IT_50 << VEML3328_OFFSET_IT);
      break;
    case 8:
      config_tmp = (VEML3328_DG_1 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_HALF << VEML3328_OFFSET_GAIN) | (VEML3328_IT_50 << VEML3328_OFFSET_IT);
      break;
    case 9:
      config_tmp = (VEML3328_DG_1 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_1 << VEML3328_OFFSET_GAIN) | (VEML3328_SENS_LOW << VEML3328_OFFSET_SENS) | (VEML3328_IT_50 << VEML3328_OFFSET_IT);
      break;
    case 10:
      config_tmp = (VEML3328_DG_1 << VEML3328_OFFSET_DG) | (VEML3328_GAIN_HALF << VEML3328_OFFSET_GAIN) | (VEML3328_SENS_LOW << VEML3328_OFFSET_SENS) | (VEML3328_IT_50 << VEML3328_OFFSET_IT);
      break;
  }

  config &= ~VEML3328_CONFIG_SENSITIVITY_MASK;
  config |= config_tmp;
  current_sensitivity_index = sensitivity_index;

  return write_config();
}

esp_err_t VEML3328::start_auto() {
  config &= ~((0b1 << VEML3328_OFFSET_SD0) | (0b1 << VEML3328_OFFSET_SD1));
  config &= ~(0b1 << VEML3328_OFFSET_AF);
  return write_config();
}

uint32_t VEML3328::get_red_in_lux() {
  return red_lux;
}

uint32_t VEML3328::get_green_in_lux() {
  return green_lux;
}

uint32_t VEML3328::get_blue_in_lux() {
  return blue_lux;
}

uint32_t VEML3328::get_clear_in_lux() {
  return clear_lux;
}

uint32_t VEML3328::get_ir_in_lux() {
  return ir_lux;
}

esp_err_t VEML3328::increase_sensitivity() {
  return set_sensitivity(current_sensitivity_index - 1);
}

esp_err_t VEML3328::decrease_sensitivity() {
  return set_sensitivity(current_sensitivity_index + 1);
}




