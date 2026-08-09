# bare-metal-firmware-learning
```description```-This repository contains my independent self-study projects in low-level firmware engineering, developed to prepare for Graduate Studies (M.Sc. in Electronics Engineering) in Italy
Because the pandemic restricted physical laboratory access during my undergraduate studies at UEM Kolkata, I used the **Wokwi** simulation platform to bypass high-level software wrappers (like Arduino or HAL libraries). Instead, I focused entirely on writing direct **Memory-Mapped I/O** register code using C++ pointers and bitwise manipulation.

**topics learnt** : bit manipulation, Memory-Mapped I/O, volatile keyword, Bitwise Masking, set/clear operation.

`**FILES DESCRIPTION** :-`

**`ESP_32_simulation_01/main.cpp`** -  Programming modern 32-bit industrial architectures (ESP32) by writing direct  switches (`1 << Pin`) to dedicated hardware SET/CLEAR registers for single cycle.
The code configures **GPIO Pin 2 as an Output** to drive an external status LED.
