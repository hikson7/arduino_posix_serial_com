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

constexpr char arduino_device_path[] = "/dev/tty.usbserial-1410";

auto main(int argc, char **argv)->int {
    SerialDescriptor sd(arduino_device_path, 9600);
    sd.open();
    sd.setTimeoutMicroSec(50);
    char reply[2];

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    std::array<unsigned char, 8> message;
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
    std::cout << "sending message: " << message[6] << std::endl;
    sd.write(&message, sizeof(unsigned char)*message.size());
    sd.read(&reply, 2);
    std::cout << (uint8_t) *reply << std::endl;

    return 0;
}
