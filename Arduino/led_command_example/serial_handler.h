/**
 * @file serial_handler.h
 * @author Hikari Hashida
 * @brief Serial handler class.
 * @version 0.1
 * @date 2021-04-28
 * 
 * last visited: 2021-04-28
 * 
 */

#ifndef SERIAL_HANDLER_H_
#define SERIAL_HANDLER_H_
#include "LEDController.h"
#include "msg_type.h"

// #define DEBUG_SERIAL

constexpr static char MARKER_START = '<';
constexpr static char MARKER_END = '>';
constexpr static char MARKER_HALT = '!';
constexpr static uint8_t MAX_MSG_LEN = 20;

// states of serial handler state machine.
enum SerialState {
    SERIAL_START,
    SERIAL_ONGOING,
    SERIAL_HALT,
    SERIAL_STANDBY
};

// Structure containing address of the command data and size of the structure.
struct CommandInfo {
    char* addr_ptr;
    size_t size;
};

/**
 * Serial handler class. Data extraction is abstracted and are isolated from classes
 * that require the data. By passing data structure address and size, hex char from
 * serial stream is stored byte by byte into the data structure.
 * 
 *  - execute(): main state machine. Changes state according to hexchar received.
 *  - addTarget(): add target class data struct to the serial handler.
 *  - setTarget(): set command type to select the data structure added previously.
 *  - appendNibble(): process hexchar to a nibble, combine to a byte and store to data structure.
 */
class SerialHandler {
 public:
    SerialHandler();
    enum Command execute();
    void addTarget(enum Command cmd_id, char* addr_ptr, size_t size);
 private:
    void setTarget(enum Command cmd_id);
    void appendNibble(char hexchar);

    /**
     *  - cur_target_type_: current extraction target message type
     *  - state_:       Current serial handler state
     *  - byte_ptr_:    Current byte pointer position.
     *  - cmd_struct_info_: data structure pointers to store the serial command data.
     */
    enum Command cur_target_type_;
    enum SerialState state_;
    char* byte_ptr_;
    struct CommandInfo cmd_struct_info_[CMD_NUM_DATA];
    // add more here ...
};
#endif  // SERIAL_HANDLER_H_
