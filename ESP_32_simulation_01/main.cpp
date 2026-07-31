volatile uint32_t* gpio_enable = (volatile uint32_t*)0x3FF44020;
volatile uint32_t* gpio_set    = (volatile uint32_t*)0x3FF44008;
volatile uint32_t* gpio_clear  = (volatile uint32_t*)0x3FF4400C;

void setup() {
  // put your setup code here, to run once:
  *gpio_enable |= (1<<2);
}

void loop() {
  *gpio_set = (1 << 2);
  delay(2000);
  *gpio_clear = (1<<2);
  delay(2000);
}
