# bare-metal-firmware-learning
```description```-This repository contains my independent self-study projects in low-level firmware engineering, developed to prepare for Graduate Studies (M.Sc. in Electronics Engineering) in Italy
I used the **Wokwi** simulation platform to bypass high-level software wrappers (like Arduino or HAL libraries). Instead, I focused entirely on writing direct **Memory-Mapped I/O** register code using C++ pointers and bitwise manipulation.

**topics learnt** : bit manipulation, Memory-Mapped I/O, volatile keyword, Bitwise Masking, set/clear operation,Physical switch mechanics and Mechanical Contact Bouncing, Software Debouncing Filters using system tick counters, Analog Peripheral Architecture (Successive Approximation Registers - SAR ADC), Signal Attenuation configuration for physical voltage shields, Digital Quantization step math, Inter-Task Communication (IPC) & FreeRTOS Queues, 

`**FILES DESCRIPTION** :-`

## **`ESP_32_simulation_01/main.cpp`** -  
Programming modern 32-bit industrial architectures (ESP32) by writing direct  switches (`1 << Pin`) to dedicated hardware SET/CLEAR registers for single cycle.
The code configures **GPIO Pin 2 as an Output** to drive an external status LED.

## **`ESP32_IDF_RTOS_Blink`** - 
This folder contains the same functional blinking LED application as `ESP_32_simulation_01`, but rewritten using Espressif's professional *ESP-IDF framework* and *FreeRTOS*.

## **`ESP32_IDF_Interactive_Button_led.cpp`** -
This project is an experiment with hardware interrupts on the ESP32 using the ESP-IDF framework.
  Configured GPIO 4 as an input button and turned on the internal pull-up resistor.
Set up a hardware interrupt trigger on the negative edge (falling voltage) to capture the exact moment the button is pushed down.
Created an Interrupt Service Routine (ISR) function using `IRAM_ATTR` so it runs directly from internal RAM. 
Fixed a core  crash (`abort() called`) by replacing standard `printf` with `esp_rom_printf`, learning that blocking functions are forbidden inside real-time interrupts.

##  **`ESP_32_button_led_02/main.cpp`** - 
This project focuses on handling physical hardware inputs on the ESP32 using the official ESP-IDF framework, moving away from CPU-heavy polling loops to asynchronous, event-driven real-time events.Initially, I set up physical GPIO 4 as a button input with an internal pull-up resistor and mapped it to a negative edge trigger (`GPIO_INTR_NEGEDGE`). I wrote a basic Interrupt Service Routine (ISR) handler using the `IRAM_ATTR` attribute to instantly flip an LED state (`led_state = !led_state`). Instead of polling(using IF statement to check whether the led is on infinite times), I used hardware interrupt to save battery and cpu clock cycles.

**Part 2: (Software Debouncing Update)**
To stabilize the circuit without adding physical hardware capacitors, I updated the primary script to include a custom software debouncer. Utilized `xTaskGetTickCountFromISR()` to poll the internal FreeRTOS system clock directly.

## **`ESP32_FreeRTOS_Multitasking/main.c`** - 
I  created two separate tasks: I wrote one function to handle blinking the LED (led_blink_task) and a second function to print text (print_task) so they run completely independently. I shared the CPU evenly: I gave both tasks the exact same priority level. This forces the chip to rapidly switch back and forth between them every millisecond so it feels like they are running at the exact same time.

## **`Industrial_Thermal_Monitor`**
This project implements an analog signal processing sub-system that mimics an industrial boiler thermal monitoring node using the official ESP-IDF framework on the ESP32.I configured the ADC Unit 1 on Channel 6 (physical Pin 34) using an attenuation setting of 12dB. This hardware adjustment expands the measurement scale of the internal transistors, allowing the chip to safely sample voltages from 0V up to 3.3V without saturating or damaging the internal circuits.

I built a mathematical translation layer inside the software. It takes raw quantization steps from the 12-bit SAR converter (0 to 4095) and processes them into precise voltage float values, which are then mapped to real-world industrial temperature data ranging from 0 to 200 degrees Celsius.

I created an autonomous safety threshold loop. When the calculated temperature crosses 120 degrees Celsius, the code instantly bypasses standard task timings to fire a rapid safety warning by flashing the status LED on Pin 2.

