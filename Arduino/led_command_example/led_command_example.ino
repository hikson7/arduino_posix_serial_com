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
#include "./serial_handler.h"
constexpr static uint8_t DELAY_VALUE = 20;  // [ms]
constexpr static uint16_t TIMER_PRELOADER_VALUE = 34286;

LEDController lc = LEDController();
SerialHandler sh = SerialHandler();

volatile bool led_on;  // valatile since led_on value can be changed out of scope i.e. potentially changes at every timer overflow.
/**
 * @brief interrupt service routine for timer overflow. Used for LED flickering every 0.5 [s].
 */

ISR(TIMER1_OVF_vect) {
  TCNT1 = TIMER_PRELOADER_VALUE;
  if (lc.isFlickerModeOn()) {
    if (led_on) {
      lc.showLEDStatus();
    } else {
      lc.turnOffLEDStatus();
     }
    led_on = !led_on;
  } else {
    lc.showLEDStatus();
  }
}

void setup() {
  Serial.begin(9600);
  lc.setBrightness(20);
  
  sh.addTarget(LED_CMD, lc.getStructAddr());
  char* ptr = lc.getStructAddr();
    /* timer interrupt settings */
  noInterrupts();  // disable interrupt
  /* intialise Timer1 on Arduino */
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = TIMER_PRELOADER_VALUE;  // preload timer
  TCCR1B |= (1 << CS12);          // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);         // enable timer overflow interrupt ISR
  interrupts();   // renable interrupt service
}

void loop() {
  sh.execute();
  delay(DELAY_VALUE);  
}
