#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "esp_adc/adc_oneshot.h"
 
#define LED 2
#define SENSOR ADC_CHANNEL_6

adc_oneshot_unit_handle_t my_adc;
QueueHandle_t thermal_queue;

void sensor_task(void *data)
{
  int reading = 0;
  while(1)
  {
    adc_oneshot_read(my_adc,SENSOR,&reading);
    float voltage = (reading/4095.0f)*3.3f;
    float temperature = (voltage/3.3f)*200;
    xQueueOverwrite(thermal_queue, &temperature);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

  }
}


void display_log_task(void *data)
{
      float received_temperature = 0.0f;
      while(1)
    {
        if (xQueuePeek(thermal_queue,&received_temperature,portMAX_DELAY)==pdPASS)
        {
            printf("Boiler Temperature: %.1f C\n", received_temperature);
          
         vTaskDelay(1000 / portTICK_PERIOD_MS);
            
        }
    }
}


void safety_task(void *data)
{
  float safety_temperature = 0.0f;
  while(1){
    if(xQueuePeek(thermal_queue,&safety_temperature,portMAX_DELAY)==pdPASS)
  {
    if(safety_temperature > 120.0f) 
      {
        printf("Safety Threshold crossed! LED Forced HIGH!\n");
        gpio_set_level(LED, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        gpio_set_level(LED, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
    else 
      {
        gpio_set_level(LED, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
  }
}
}
void app_main(void)
{
    gpio_reset_pin(LED);
    gpio_set_direction(LED,GPIO_MODE_OUTPUT);
    gpio_set_level(LED,0);
    adc_oneshot_unit_init_cfg_t unit_form = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&unit_form, &my_adc);
    adc_oneshot_chan_cfg_t pin_form = { .atten = ADC_ATTEN_DB_12, .bitwidth = ADC_BITWIDTH_12 };
    adc_oneshot_config_channel(my_adc, SENSOR, &pin_form);
    thermal_queue = xQueueCreate(1, sizeof(float));
    xTaskCreate(sensor_task, "Sensor_Task", 2048, NULL, 2, NULL);
    xTaskCreate(display_log_task, "Display_Task", 2048, NULL, 2, NULL);
    xTaskCreate(safety_task,"safety",2048,NULL,3,NULL);
    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
