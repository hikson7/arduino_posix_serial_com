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
// #include <array>
#include <chrono>
#include <thread>

#include "sercom.h"

constexpr char arduino_device_path[] = "/dev/tty.usbserial-1410";
// constexpr char arduino_device_path[] = "/dev/tty.usbmodem14101";

auto main(int argc, char **argv)->int {
    SerialDescriptor sd(arduino_device_path, 9600);
    sd.open();
    /* timeout < 1 [s] causes places no bytes to buffer */
    sd.setTimeoutSec(2);
    const size_t BUFFER_SIZE = 50;
    char reply[BUFFER_SIZE];
    memset(reply, '\0', BUFFER_SIZE);

    // std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    const size_t MESSAGE_LEN = 20;
    // const size_t MESSAGE_SIZE = 5;

    // std::array<unsigned char, MESSAGE_SIZE> message;
    // std::string message = "<10AFFFFFF>";
    // if (argc == 5+1) {
    //     message.clear();
    //     message.append("<");
    //     message.append(argv[1]);  // blink
    //     message.append(argv[2]);  // brightness
    //     message.append(argv[3]);  // red_val
    //     message.append(argv[4]);  // green_val
    //     message.append(argv[5]);  // blue_val
    //     message.append(argv[6]);  //
    //     message.append(">");
    // }
    // if (argc == 4) {
    //     message.at(0) = static_cast<unsigned char> ('<');
    //     message.at(1) = static_cast<unsigned char> (std::atoi(argv[1]));
    //     message.at(2) = static_cast<unsigned char> (std::atoi(argv[2]));
    //     message.at(3) = static_cast<unsigned char> (std::atoi(argv[3]));
    //     message.at(4) = static_cast<unsigned char> ('>');
    // }
    // char* c_message = new char[message.size()];
    // memcpy(c_message, message.data(), message.size());
    // std::cout << c_message;
    // char c_message[] = "<10AFFFFFF>";
    //char c_message[] = "<10A000000>";
    // char c_message[] = "<0010A0000FFFF>";
    //  = 0         01          0A              00      00      FF
    //  = mode=0, blink=true, red=0, green=0, blue=255,  brightness=10, 
    char message[MESSAGE_LEN];
    while (1) {
        memset(message, '\0', MESSAGE_LEN);
        printf("Usage: insert values in {HERE} without space <{MODE_ID}{BLINK}{BRIGHTNESS}{RED}{GREEN}{BLUE}{DURATION}>\n->");
        scanf("%s", message);
        sd.write(&message, sizeof(unsigned char)*MESSAGE_LEN);

        // int nbytes = sd.read(&reply, BUFFER_SIZE);
        // printf("%s\n", reply);
        // memset(reply, '\0', BUFFER_SIZE);
    }
    // delete[] c_message;
    return 0;
}
