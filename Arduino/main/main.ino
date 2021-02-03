/**
 * @file indicator.ino
 * @author Hikari Hashida
 * @brief Arduino code to demonstrate simple multi-byte serial communicaton protocol.
 * @version 0.1
 * @date 2021-02-03
 * 
 */

#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "./LEDController.h"

constexpr static uint8_t DELAY_VALUE = 20;  // [ms]

LEDController lc = LEDController();

void setup() {
  Serial.begin(9600);
  lc.setBrightness(20);
}

void loop() {

  lc.readDataFromSerial();
  lc.indicateLED();

  delay(DELAY_VALUE);  
}
