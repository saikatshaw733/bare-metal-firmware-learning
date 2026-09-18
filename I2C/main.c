#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_adc/adc_oneshot.h"
 
#define SDA 21
#define SCL 22
#define I2C_MASTER I2C_NUM_0
#define FREQ 100000
#define OLED 0X3C
#define LED 2
#define SENSOR ADC_CHANNEL_6

adc_oneshot_unit_handle_t my_adc;
QueueHandle_t thermal_queue;

void sensor_task(void *data)
{
  int reading = 0;
  while(1)
  {
    adc_oneshot_read(my_adc,sensor,&reading);
    float voltage = (reading/4095.0)*3.3f;
    float temperature = (voltage/3.3f)*200;
    xQueueSend(thermal_queue,&temperature,0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);    
  }
}

void display_task(void *data)
{
  float telemetry = 0.0f;
  if(xQueueReceive(thermal_queue,&telemetry,portMax_DELAY)==pdPASS)
  {
    i2c_cmd_handle_t command_link= i2c_cmd_link_create();
    i2c_master_start(command_link);
    i2c_master_write_byte(command_link,0x40,true);
    i2c_master_stop(command_link);
    i2c_master_cmd_begin(I2C_MASTER,command_link,100 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(command_link);
    if(telemetry_data > 120.0f) 
    {
      gpio_set_level(LED, 1);
      vTaskDelay(500/portTICK_period_MS);
      gpio_set_level(LED, 0);
      vTaskDelay(500/portTICK_period_MS);
    }
}
}
void i2c(void)
{
  i2c_config_t config = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = SDA,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_io_num = SCL,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = FREQ
  };

i2c_param_config(I2C_MASTER,&config);
i2c_driver_install(I2C_MASTER,config.mode,0,0,0);
}
void app_main(void)
{
    gpio_reset_pin(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_level(LED, 0);

    adc_oneshot_unit_init_cfg_t unit_form = { .unit_id = ADC_UNIT_1 };
    adc_oneshot_new_unit(&unit_form, &my_adc);
    adc_oneshot_chan_cfg_t pin_form = { .atten = ADC_ATTEN_DB_12, .bitwidth = ADC_BITWIDTH_12 };
    adc_oneshot_config_channel(my_adc, SENSOR, &pin_form);
    i2c();
    thermal_queue = xQueueCreate(5, sizeof(float));
    xTaskCreate(sensor_task,"sensor",2048,NULL,2,NULL);
    xTaskCreate(display_task, "Display_Task", 2048, NULL, 2, NULL);
    while(1)
    {
      vTaskDelay(1000/portTICK_PERIOD_MS):
    }
}