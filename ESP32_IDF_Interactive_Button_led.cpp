#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_PIN 4

static void IRAM_ATTR button_isr_handler(void* arg) 
{
    esp_rom_printf("Button Pressed! Shoulder tap successful!\n");
}

void app_main(void)
{
    
    gpio_reset_pin(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    gpio_reset_pin(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT);
    gpio_pullup_en(GPIO_NUM_4);
    gpio_set_intr_type(GPIO_NUM_4, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_NUM_4, button_isr_handler, (void*) BUTTON_PIN);

    while(1)
    {
        
        gpio_set_level(GPIO_NUM_2, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(GPIO_NUM_2, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
