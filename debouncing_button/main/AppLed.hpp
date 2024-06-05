#pragma once
#include "driver/gpio.h"
#include <esp_log.h>

class AppLed {

private:
    gpio_num_t gpio_led;

public:

    AppLed(gpio_num_t gpio_pin): gpio_led(gpio_pin){}

  void init(void) {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << this->gpio_led);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    // ESP_LOGI("init","GPIO_BUTTON: %d",this->gpio_button);
  }

  void set(bool val)
    {
    gpio_set_level(gpio_led, static_cast<uint32_t>(val));
    }
};
