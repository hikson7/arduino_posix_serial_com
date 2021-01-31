
#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <thread>

#include "sercom.h"

auto main(void)->int {
    std::cout << "hello world!" << std::endl;

    SerialDescriptor sd("/dev/tty.usbserial-1410", 9600);
    sd.open();
    unsigned char reply;

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    // std::string message;
    // message.append("<");
    // message.append(static_cast<char>(5));
    // message.append(static_cast<char>(false));
    // message.append(static_cast<char>());
    // message.append(">");
    std::array<unsigned char, 7> message;
    message.at(0) = static_cast<unsigned char> ('<');
    message.at(1) = static_cast<unsigned char> (5);
    message.at(2) = static_cast<unsigned char> (false);
    message.at(3) = static_cast<unsigned char> (150);
    message.at(4) = static_cast<unsigned char> (150);
    message.at(5) = static_cast<unsigned char> (0);
    message.at(6) = static_cast<unsigned char> ('>');

    sd.write(&message, sizeof(unsigned char)*message.size());
    sd.read(&reply, 2);
    std::cout << static_cast<char> (reply) << std::endl;

    return 0;
}