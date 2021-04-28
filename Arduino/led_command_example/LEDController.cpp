/**
 * @file LEDController.cpp
 * @author Hikari Hashida
 * @brief  Arduino code to demonstrate simple multi-byte serial communicaton protocol. Indicated via WS2812 LEDs.
 * @version 0.1
 * @date 2021-01-29
 * last visited: 2021-04-28
 */
#include <Adafruit_NeoPixel.h>
#include "./LEDController.h"

LEDController::LEDController() {
    led_indicator_ = Adafruit_NeoPixel(NUM_INDIC_PIXELS,
    PIN_INDIC, NEO_GRB + NEO_KHZ800);
    led_indicator_.begin();
}

char* LEDController::getStructAddr(void) {
    return static_cast<char*>(static_cast<void*>(&lcc_));
}

size_t LEDController::getStructSize(void) {
    return sizeof(struct LEDControllerCommand);
}

void LEDController::setDataReady(void) {
    is_new_data_ = true;
}

void LEDController::setBrightness(uint8_t brightness) {
  led_indicator_.setBrightness(brightness);
}

void LEDController::showLEDStatus(void) {
    setAndShowLEDStatus(lcc_.red_val, lcc_.green_val,
                     lcc_.blue_val, lcc_.brightness);
}
void LEDController::turnOffLEDStatus(void) {
    setAndShowLEDStatus(0, 0, 0, 0);
}

void LEDController::setAndShowLEDStatus(uint8_t red_val,
             uint8_t green_val, uint8_t blue_val, uint8_t brightness) {
    #ifdef DEBUG_LED
    Serial.println("===== RESULT ===== ");
    Serial.print("blink=");
    Serial.println(lcc_.blink);
    Serial.print("brightness=");
    Serial.println(lcc_.brightness);
    Serial.print("red_val=");
    Serial.println(lcc_.red_val);
    Serial.print("green_val=");
    Serial.println(lcc_.green_val);
    Serial.print("blue_val=");
    Serial.println(lcc_.blue_val);
    Serial.print("duration=");
    Serial.println(lcc_.duration);
    Serial.println("===== ===== =====");
    #endif

    setBrightness(brightness);
    for (int i = 0; i < NUM_INDIC_PIXELS; i++) {
        led_indicator_.setPixelColor(i, red_val, green_val, blue_val);
    }
    led_indicator_.show();
}

bool LEDController::isFlickerModeOn(void) {
    return lcc_.blink;
}
