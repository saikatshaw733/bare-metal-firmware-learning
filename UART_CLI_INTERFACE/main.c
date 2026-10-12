#include<stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#define UART_PORT UART_NUM_0
#define BUFF_SIZE 1024

void uart_initialize(void)
{
    uart_config_t uart_setup={
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};
    uart_param_config(UART_PORT, &uart_setup);
    uart_driver_install(UART_PORT,BUFF_SIZE*2,0,0,NULL,0);
}