#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/timers.h"
#include "AppLed.hpp"
#include "AppButton.hpp"
#include <esp_log.h>


#define BUTTON_GPIO_PIN GPIO_NUM_21
#define LED_GPIO_PIN GPIO_NUM_5
#define DEBOUNCE_TIME_MS 10  // Tempo di debounce in millisecondi


extern "C" void app_main(){
	AppLed my_led(LED_GPIO_PIN);
	auto fn = [&my_led](bool state){ my_led.set(state); };
	AppButton button(fn,BUTTON_GPIO_PIN);

	my_led.init();
	button.init();
	vTaskSuspend(nullptr);
	while (true) {
         vTaskDelay(pdMS_TO_TICKS(1000));
    }
}