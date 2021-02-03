/**
 * @file LEDController.h
 * @author Hikari Hashida
 * @brief Simple class that receives commands from serial input via USB/UART, and outputs to LED strip.
 * @version 0.1
 * @date 2021-02-03
 * 
 */

#include <Adafruit_NeoPixel.h>

/* Status indicator command format stored in struct */
struct __attribute__((__packed__)) LEDControllerCommand {
    uint8_t duration;
    uint8_t blink;
    uint8_t red_val;
    uint8_t green_val;
    uint8_t blue_val;
    uint8_t brightness;
};

constexpr static uint8_t PIN_INDIC = 12;
constexpr static uint8_t NUM_INDIC_PIXELS = 8;
constexpr static uint8_t START_MARKER = '<';
constexpr static uint8_t END_MARKER = '>';

class LEDController {
 public:
    LEDController();
    void readDataFromSerial();
    void indicateLED();
    void setBrightness(uint8_t brightness);
    void setAndShowLEDStatus(uint8_t red_val,
         uint8_t green_val, uint8_t blue_val);
 private:
    /* status indicator struct to store the indication parameters */
    LEDControllerCommand lcc_;
    /* byte-sized pointer to keep track of the place in struct */
    uint8_t* byte_ptr_;
    /* data receiving in progress */
    bool in_progress_;
    bool is_new_data_ = false;
    Adafruit_NeoPixel led_indicator_;
};
