/**
 * @file INDICator.cpp
 * @author Hikari Hashida
 * @brief  Arduino code to demonstrate simple multi-byte serial communicaton protocol. Indicated via WS2812 LEDs.
 * @version 0.1
 * @date 2021-01-29
 *      
 */
#include <Adafruit_NeoPixel.h>

#include "./LEDController.h"

LEDController::LEDController() {
    led_indicator_ = Adafruit_NeoPixel(NUM_INDIC_PIXELS,
    PIN_INDIC, NEO_GRB + NEO_KHZ800);
    led_indicator_.begin();
}

/**
 * @brief Read and extract rover status indication data from serial.
 * As stream of data is received byte-by-byte, it will be placed straight into LEDControllerCommand struct. 
 * 
 */
void LEDController::readDataFromSerial() {
    if (Serial.available() <= 0) return;

    uint8_t received_byte = Serial.read();
    switch (received_byte) {
        case START_MARKER:
            if (!in_progress_) {
                /* if receiving data not in progress, initiate */
                in_progress_ = true;
                /* set byte pointer to point at the start of struct memory */
                byte_ptr_ = static_cast<uint8_t*>(&lcc_.duration);
            }
            break;
        case END_MARKER:
            if (in_progress_) {
                /* if receving data in progress, end it and set new data flag */
                in_progress_ = false;
                is_new_data_ = true;
            }
            break;
        default:
            if (in_progress_) {
                /* insert received_byte into the struct memory, and move 
                    the pointer to next byte */
                *(byte_ptr_++) = received_byte;
            }
    }
    /* 
    // For debuggig, send OK message back to main computer.
    if (is_new_data_) {
        Serial.write("OK");
    }
    */
}

/**
 * @brief Indicate led according to the serial message received. LED indication will be executed once per command.
 * Since all received data are stored in struct, they can be accessed through lcc_ private variable.
 */
void LEDController::indicateLED() {
    if (!is_new_data_) return;

    setBrightness(lcc_.brightness);
    /* Light LED for given duration in [s] */
    for (int i = 0; i < lcc_.duration; i++) {
        setAndShowLEDStatus(lcc_.red_val, lcc_.green_val, lcc_.blue_val);
        if (lcc_.blink) {
            delay(500);
            setAndShowLEDStatus(0, 0, 0);
            delay(500);
        } else {
            delay(1000);
        }
    }

    setAndShowLEDStatus(0, 0, 0);
    is_new_data_ = false;
}

void LEDController::setBrightness(uint8_t brightness) {
  led_indicator_.setBrightness(brightness);
}

void LEDController::setAndShowLEDStatus(
    uint8_t red_val, uint8_t green_val, uint8_t blue_val) {
    for (int i = 0; i < NUM_INDIC_PIXELS; i++) {
        led_indicator_.setPixelColor(i, red_val, green_val, blue_val);
    }
    led_indicator_.show();
}
