#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_PIN 4
#define LED_PIN 2
volatile int led_state = 0;
static uint32_t last_time = 0;

static void IRAM_ATTR button_isr_handler(void* arg) 
{
    uint32_t current_time = xTaskGetTickCountFromISR();
    uint32_t debounce = 200/portTICK_PERIOD_MS;
    if((current_time - last_time) > debounce)
    {
      led_state = !led_state;
      gpio_set_level(GPIO_NUM_2, led_state);
      last_time = current_time;
    }
    
}

void app_main(void)
{
    gpio_reset_pin(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_2, led_state);

    
    gpio_reset_pin(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_NUM_4);
    gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_4, button_isr_handler, (void*) BUTTON_PIN);

    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
    }
}
