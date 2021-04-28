

#ifndef SERIAL_HANDLER_H_
#define SERIAL_HANDLER_H_
#include "LEDController.h"
// #define DEBUG_SERIAL

enum Command {
    LED_CMD
};

enum SerialState {
    SERIAL_START,
    SERIAL_ONGOING,
    SERIAL_HALT,
    SERIAL_STANDBY
};

constexpr static char MARKER_START = '<';
constexpr static char MARKER_END = '>';
constexpr static uint8_t MAX_MSG_LEN = 20;

class SerialHandler {
 public:
    SerialHandler();
    void execute();
    void addTarget(enum Command cmd_id, char* struct_ptr);
 private:
    void setBytePtr(char msg_type);
    char* lcc_ptr_;
    char* byte_ptr_;
    /* status indicator struct to store the indication parameters */
    /* data receiving in progress */
    enum SerialState state_;
};
#endif  // SERIAL_HANDLER_H_
