#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

#define LED 2
#define SENSOR ADC_CHANNEL_6

adc_oneshot_unit_handle_t my_adc;

void app_main(void)
{

  gpio_reset_pin(LED);
  gpio_set_direction(LED,GPIO_MODE_OUTPUT);
  gpio_set_level(LED,0);
  adc_oneshot_unit_init_cfg_t unit_form = {

  .unit_id = ADC_UNIT_1
  };
  adc_oneshot_new_unit(&unit_form,&my_adc);
  adc_oneshot_chan_cfg_t pin_form = {
    
    .atten = ADC_ATTEN_DB_12,
    .bitwidth = ADC_BITWIDTH_12,

  };
  adc_oneshot_config_channel(my_adc,SENSOR,&pin_form);
  int reading = 0;
  while(1){
    adc_oneshot_read(my_adc,SENSOR,&reading);

    float voltage = (reading / 4095.0)*3.3f;
    float temperature = (voltage / 3.3)*200.0f;
    printf("voltage :- %.1f Volt| Temperature :- %.1f degree celcius\n",voltage,temperature);
    if(temperature > 120)
    {
      printf("[WARNING] Critical Boiler Temperature Exceeded! Triggering LED Light!\n");
      gpio_set_level(LED,1);
        vTaskDelay(300/portTICK_PERIOD_MS);
      gpio_set_level(LED,0);
        vTaskDelay(300/portTICK_PERIOD_MS);

    }
    else{
           
      gpio_set_level(LED,0);
      vTaskDelay(3000/portTICK_PERIOD_MS);
    }

  }
}
