/**
 * @file test_main.cpp
 * @author Hikari Hashida
 * @brief Very basic serial com tester
 * @version 0.1
 * @date 2021-01-31
 * 
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <thread>

#include "sercom.h"

// constexpr char arduino_device_path[] = "/dev/tty.usbserial-1410";
constexpr char arduino_device_path[] = "/dev/tty.usbmodem14101";

auto main(int argc, char **argv)->int {
    SerialDescriptor sd(arduino_device_path, 9600);
    sd.open();
    /* timeout < 1 [s] causes places no bytes to buffer */
    sd.setTimeoutSec(1);
    const size_t BUFFER_SIZE = 3;
    char reply[BUFFER_SIZE];

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    const size_t MESSAGE_SIZE = 8;
    // const size_t MESSAGE_SIZE = 5;

    std::array<unsigned char, MESSAGE_SIZE> message;
    if (argc == 7) {
        message.at(0) = static_cast<unsigned char> ('<');
        message.at(1) = static_cast<unsigned char> (std::atoi(argv[1]));
        message.at(2) = static_cast<unsigned char> (std::atoi(argv[2]));
        message.at(3) = static_cast<unsigned char> (std::atoi(argv[3]));
        message.at(4) = static_cast<unsigned char> (std::atoi(argv[4]));
        message.at(5) = static_cast<unsigned char> (std::atoi(argv[5]));
        message.at(6) = static_cast<unsigned char> (std::atoi(argv[6]));
        message.at(7) = static_cast<unsigned char> ('>');
    } else {
        std::cout << "default message" << std::endl;
        message.at(0) = static_cast<unsigned char> ('<');
        message.at(1) = static_cast<unsigned char> (5);
        message.at(2) = static_cast<unsigned char> (false);
        message.at(3) = static_cast<unsigned char> (255);
        message.at(4) = static_cast<unsigned char> (255);
        message.at(5) = static_cast<unsigned char> (255);
        message.at(6) = static_cast<unsigned char> ('>');
    }
    // if (argc == 4) {
    //     message.at(0) = static_cast<unsigned char> ('<');
    //     message.at(1) = static_cast<unsigned char> (std::atoi(argv[1]));
    //     message.at(2) = static_cast<unsigned char> (std::atoi(argv[2]));
    //     message.at(3) = static_cast<unsigned char> (std::atoi(argv[3]));
    //     message.at(4) = static_cast<unsigned char> ('>');
    // }

    sd.write(&message, sizeof(unsigned char)*message.size());
    int nbytes = sd.read(&reply, BUFFER_SIZE);
    std::cout << "bytes read: " <<  nbytes << std::endl;

    /* uint8_t or int8_t does not work. use size_t or int */
    std::cout << (size_t) reply[0] << std::endl;

    return 0;
}
