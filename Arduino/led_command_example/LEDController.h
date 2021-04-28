/**
 * @file LEDController.h
 * @author Hikari Hashida
 * @brief Simple class that receives commands from serial input via USB/UART, and outputs to LED strip.
 * @version 0.1
 * @date 2021-02-03
 * last visited: 2021-04-28
 */
#ifndef LED_CONTROLLER_H_
#define LED_CONTROLLER_H_
#include <Adafruit_NeoPixel.h>
// #define DEBUG_LED

/* Status indicator command format stored in struct */
struct __attribute__((__packed__)) LEDControllerCommand {
    bool blink;
    uint8_t brightness;
    uint8_t red_val;
    uint8_t green_val;
    uint8_t blue_val;
    uint8_t duration;
};
constexpr static uint8_t PIN_INDIC = 12;
constexpr static uint8_t NUM_INDIC_PIXELS = 8;


class LEDController {
 public:
    LEDController();
    void indicateLED(void);
    void setBrightness(uint8_t brightness);
    void turnOffLEDStatus(void);
    void showLEDStatus(void);
    void setAndShowLEDStatus(uint8_t red_val,
         uint8_t green_val, uint8_t blue_val, uint8_t brightness);
    bool isFlickerModeOn(void);

    // serial data related functions.
    char* getStructAddr(void);
    size_t getStructSize(void);
    void setDataReady(void);
 private:
    /* status indicator struct to store the indication parameters */
    LEDControllerCommand lcc_;
    Adafruit_NeoPixel led_indicator_;
    bool is_new_data_;
};

#endif 
