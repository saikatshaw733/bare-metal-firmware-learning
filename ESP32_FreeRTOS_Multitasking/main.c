#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN 2

void led_blink_task(void* data)
{
  gpio_reset_pin(LED_PIN);
  gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT);
  while(1)
  {

    gpio_set_level(LED_PIN,1);
    vTaskDelay(500/portTICK_PERIOD_MS);

    gpio_set_level(LED_PIN,0);
    vTaskDelay(500/portTICK_PERIOD_MS);

  }
}

void print_task(void* data)
{
  while(1)
  {
    printf("led is working");
    vTaskDelay(3000/portTICK_PERIOD_MS);
  }
}

void app_main(void)
{
  xTaskCreate(led_blink_task,"blinking",2048,NULL,2,NULL);
  xTaskCreate(print_task,"printing",2048,NULL,2,NULL);
  while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}