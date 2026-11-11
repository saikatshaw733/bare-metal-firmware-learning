#include <stdio.h>
#include <string.h> 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_vfs_dev.h" 

#define UART_PORT UART_NUM_0
#define BUFF_SIZE 1024

float default_setpoint = 120.0f; 

void uart_initialize(void)
{
    uart_config_t uart_setup = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_PORT, &uart_setup);
    uart_driver_install(UART_PORT, BUFF_SIZE * 2, 0, 0, NULL, 0);
    esp_vfs_dev_uart_register();
    esp_vfs_dev_uart_use_driver(0);
}

void uart_receiver(void *parameter)
{
    uint8_t rx_memory[BUFF_SIZE];
    
    printf("commands available - SETPOINT <Temperature>\n");
    
    while(1)
    {
        memset(rx_memory, 0, sizeof(rx_memory));
        
        int length = uart_read_bytes(UART_PORT, rx_memory, BUFF_SIZE - 1, 50 / portTICK_PERIOD_MS);
        
        if(length > 0)
        {
            rx_memory[length] = '\0';
            
            if(strncmp((char*)rx_memory, "SETPOINT", 8) == 0)
            {
                float temp = 0.0f;
                if(sscanf((char*)rx_memory, "SETPOINT %f", &temp) == 1)
                {
                    default_setpoint = temp;
                    printf("\n[SUCCESS] Setpoint register globally updated to: %.1f C\n", default_setpoint);
                }
            }
            else 
            {
                printf("\n Command not recognized.\n");
            }
        }
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    uart_initialize();
    xTaskCreate(uart_receiver, "uart_cli", 4096, NULL, 2, NULL);
     while(1)
    {
        printf("[STATUS LOG] Active Monitoring Limit: %.1f C\n", default_setpoint);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
