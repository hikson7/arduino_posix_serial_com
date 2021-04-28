/**
 * @file serial_handler.cpp
 * @author Hikari Hashida
 * @brief Serial handler class implementation
 * @version 0.1
 * @date 2021-04-28
 * 
 * last visited: 2021-04-28
 */

#include "serial_handler.h"

/**
 * validate if received char is expected. Valid chars include any
 * hexadecimal values i.e. '0'-'9', 'a'-'f'/'A'-'F', or
 * start and end markers.
 */
static bool isValidChar(const char hexchar) {
    char hchar = toupper(hexchar);
    if ((hchar >= '0' && hchar <= '9')
            || (hchar >= 'A' && hchar <= 'F')
            || hchar == MARKER_START
            || hchar == MARKER_END
            || hchar == MARKER_HALT) {
        return true;
    } else {
        return false;
    }
}

/**
 * Convert Hexadecimal char to integer value 0~15.
 * Precondition: hexadecimal char is valid i.e. 0~F or 0~f
 */
static uint8_t hexchar2Int(const char hexchar) {
    char hchar = toupper(hexchar);
    return (hchar >= 'A') ? (hchar - 'A' + 10):(hchar - '0');
}

SerialHandler::SerialHandler() {
    // initialise array of command info data structure
    for (int i=0; i < CMD_NUM_DATA; i++) {
        cmd_struct_info_[i].addr_ptr = NULL;
        cmd_struct_info_[i].size = 0;
    }
    state_ = SERIAL_STANDBY;
}

void SerialHandler::addTarget(enum Command cmd_id,
                                     char* struct_ptr, size_t size) {
    if (cmd_id < 0 || cmd_id >= CMD_NUM_DATA) return;

    // add target data structure info.
    struct CommandInfo cinfo;
    cinfo.addr_ptr = struct_ptr;
    cinfo.size = size;
    cmd_struct_info_[cmd_id] = cinfo;
}

void SerialHandler::setTarget(enum Command cmd_id) {
    if (cmd_id < 0 || cmd_id >= CMD_NUM_DATA) return;
    cur_target_type_ = cmd_id;
    byte_ptr_ = cmd_struct_info_[cmd_id].addr_ptr;
}

void SerialHandler::appendNibble(char hexchar) {
    static uint8_t nibble_hi = 0;
    if (nibble_hi == 0) {
        nibble_hi = (uint8_t) hexchar;
    } else  {
        nibble_hi = hexchar2Int((char) nibble_hi);
        uint8_t nibble_lo = hexchar2Int(hexchar);
        *(byte_ptr_++) = (nibble_hi << 4) | nibble_lo;
        nibble_hi = 0;
    }
}

/**
 * @brief Read and extract rover status indication data from serial.
 * As stream of data is received byte-by-byte, it will be placed straight into LEDControllerCommand struct. 
 * 
 */
enum Command SerialHandler::execute() {
    // counter for number of hexchar bytes received
    static size_t byte_ctr = 0;
    if (Serial.available() <= 0) return CMD_NULL;

    #ifdef DEBUG_SERIAL
    Serial.print("byte_ptr=0x");
    Serial.println((size_t)byte_ptr_, HEX);
    #endif
    char received_byte = Serial.read();

    // Given emergency HALT command. Skip to the end.
    if (received_byte == MARKER_HALT) {
        state_ = SERIAL_HALT;
        cur_target_type_ = CMD_HALT;
    }
    // state machine
    switch (state_) {
            case SERIAL_START:
                /* set message type and move to extration state */
                #ifdef DEBUG_SERIAL
                Serial.println("\nSERIAL_START");
                #endif
                setTarget((enum Command) hexchar2Int(received_byte));
                state_ = SERIAL_ONGOING;
            break;

            case SERIAL_ONGOING:
            /* extraction state */
            #ifdef DEBUG_SERIAL
                Serial.println("\nSERIAL_ONGOING");
            #endif
                /*
                    Stop extraction if:
                    -   1. received end marker
                    -   2. appropriate mode id was not given
                    -   3. given char is not hexadecimal
                    -   4. exceeded expected message length
                    -   5. exceeded absolute maximum expected message length
                */
                if (received_byte == MARKER_HALT
                         || received_byte == MARKER_END
                         || byte_ptr_ == NULL
                         || !isValidChar(received_byte)
                         || byte_ctr >= cmd_struct_info_[cur_target_type_].size*2
                         || byte_ctr >= MAX_MSG_LEN) {
                    // stop extraction.
                    state_ = SERIAL_HALT;
                    break;
                }
                // increment hexchar byte counter. Add to byte.
                byte_ctr++;
                // extract nibble from hexchar.
                appendNibble(received_byte);
                break;

            case SERIAL_STANDBY:
                /* No ongoing extraction. Wait for start marker */
                #ifdef DEBUG_SERIAL
                Serial.println("\nSERIAL_STANDBY");
                #endif
                // waiting for start marker
                if (received_byte == MARKER_START) {
                    state_ = SERIAL_START;
                }
                break;
        }

    // jump to here for any resons to stop serial extraction.
    if (state_ == SERIAL_HALT) {
    LABEL_HALT:
        #ifdef DEBUG_SERIAL
        Serial.println("\nSERIAL_HALT");
        #endif
        // reset variable and the state
        state_ = SERIAL_STANDBY;
        byte_ptr_ = NULL;
        byte_ctr = 0;
        // return completed message type.
        return cur_target_type_;
    }
    // No data is ready yet.
    return CMD_NULL;
}
