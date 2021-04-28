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
            || hchar == MARKER_END) {
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
    lcc_ptr_ = NULL;
    state_ = SERIAL_STANDBY;
}

void SerialHandler::addTarget(enum Command cmd_id, char* struct_ptr) {
    switch (cmd_id) {
        case LED_CMD:
            lcc_ptr_ = struct_ptr;
            break;
        default:
            break;
    }
}

void SerialHandler::setBytePtr(char msg_type) {
    enum Command cmd = (enum Command) hexchar2Int(msg_type);
    switch (cmd) {
        case LED_CMD:
            byte_ptr_ = lcc_ptr_;
            break;
        default:
            byte_ptr_ = NULL;
            break;
    }
}


/**
 * @brief Read and extract rover status indication data from serial.
 * As stream of data is received byte-by-byte, it will be placed straight into LEDControllerCommand struct. 
 * 
 */
void SerialHandler::execute() {
    static uint8_t nibble_hi = 0;
    static size_t byte_ctr = 0;
    if (Serial.available() <= 0) return;

    #ifdef DEBUG_SERIAL
    Serial.print("byte_ptr=0x");
    Serial.println((size_t)byte_ptr_, HEX);
    #endif
    char received_byte = Serial.read();
    switch (state_) {
            case SERIAL_START:
                #ifdef DEBUG_SERIAL
                Serial.println("\nSERIAL_START");
                #endif
                setBytePtr(received_byte);
                state_ = SERIAL_ONGOING;
            break;

            case SERIAL_ONGOING:
            #ifdef DEBUG_SERIAL
                Serial.println("\nSERIAL_ONGOING");
            #endif
                /*
                    Stop extraction if:
                    -   1. appropriate mode id was not given
                    -   2. given char is not hexadecimal
                    -   3. absolute maximum expected message length exceeded
                */
                if (received_byte == MARKER_END
                         || byte_ptr_ == NULL
                         || !isValidChar(received_byte)
                         byte_ctr >= MAX_MSG_LEN) {
                    state_ = SERIAL_HALT;
                    break;
                }
                byte_ctr++;
                if (nibble_hi == 0) {
                    nibble_hi = (uint8_t) received_byte;
                } else  {
                    nibble_hi = hexchar2Int((char) nibble_hi);
                    uint8_t nibble_lo = hexchar2Int(received_byte);
                    *(byte_ptr_++) = (nibble_hi << 4) | nibble_lo;
                    nibble_hi = 0;
                }
            break;

            case SERIAL_STANDBY:
                #ifdef DEBUG_SERIAL
                Serial.println("\nSERIAL_STANDBY");
                #endif
                // waiting for start marker
                if (received_byte == MARKER_START) {
                    state_ = SERIAL_START;
                }
            break;
        }
    if (state_ == SERIAL_HALT) {
        #ifdef DEBUG_SERIAL
        Serial.println("\nSERIAL_HALT");
        #endif
        nibble_hi = 0;
        state_ = SERIAL_STANDBY;
        byte_ptr = NULL;
        byte_ctr = 0;
    }
}
