/**
 * @file indicator.ino
 * @author Hikari Hashida
 * @brief Arduino code to demonstrate simple multi-byte serial communicaton protocol.
 * @version 0.1
 * @date 2021-02-03
 * last visited: 2021-04-28
 */

#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "./LEDController.h"
#include "./serial_handler.h"
constexpr static uint8_t DELAY_VALUE = 20;  // [ms]
constexpr static uint16_t TIMER_PRELOADER_VALUE = 34286;  // prescaled to repeat every 500 [ms]

LEDController lc = LEDController();
SerialHandler sh = SerialHandler();

// valatile since led_on value can be changed out of scope i.e. potentially changes at every timer overflow.
volatile bool led_on;  
/**
 * interrupt service routine for timer overflow. Used for LED flickering every 0.5 [s].
 */

ISR(TIMER1_OVF_vect) {
  TCNT1 = TIMER_PRELOADER_VALUE;
  if (lc.isFlickerModeOn()) {
    (led_on) ? lc.showLEDStatus(): lc.turnOffLEDStatus();
    led_on = !led_on;
  } else {
    lc.showLEDStatus();
  }
}

void setup() {
  Serial.begin(9600);

  // give memory address to the serial handler for extracting command from serial.
  sh.addTarget(CMD_LED, lc.getStructAddr(), lc.getStructSize());

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
  // extract command if available from serial.
  // for code exectution requiring a new message in full i.e. no partial data,
  // then set data is ready.
  switch (sh.execute()) {
    case CMD_HALT:
      // do some code to stop everything.
      break;
    case CMD_LED:
      // not necessary for LED command. Placed here as an example.
      lc.setDataReady();
      break;
    case CMD_NULL:
    default:
      break;
  }

  /* Add any other executions here based on serial command */
  delay(DELAY_VALUE);
}
