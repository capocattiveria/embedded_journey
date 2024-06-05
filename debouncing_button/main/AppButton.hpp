#pragma once
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include <esp_log.h>
#include <functional>

void button_handler(void *arg);

class AppButton {
private:
  bool state;
  std::function<void(bool)> pressedHandler;
  gpio_num_t gpio_button;

public:
  AppButton(std::function<void(bool)> h, gpio_num_t pin)
      : state(false), pressedHandler(h), gpio_button(pin) {}

  void init(void) {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << this->gpio_button);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    ESP_LOGI("init","GPIO_BUTTON: %d",this->gpio_button);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(this->gpio_button, button_handler, this);
  }

  void toggle(void) {
    state = !state;
    pressedHandler(state);
  }
};

//allocate the funcion in the internal RAM
IRAM_ATTR void button_handler(void *arg) {

    //do not log in interrupt function
    // ESP_LOGI("button_handler","pressed");

    //volatile it means is not to be optimized from the compiler, the variable
    // can be modified from an interrupt
  static volatile TickType_t next = 0;
  TickType_t now = xTaskGetTickCountFromISR();
  if (now > next) {
    auto btn = reinterpret_cast<AppButton *>(arg);
    btn->toggle();
    next = now + 500 / portTICK_PERIOD_MS;
  }
}