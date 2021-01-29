
#include <iostream>
#include <chrono>
#include <thread>

#include "sercom.h"

auto main(void)->int {
    std::cout << "hello world!" << std::endl;

    SerialDescriptor sd("/dev/tty.usbmodem14101", 9600);
    sd.open();
    unsigned char handshake = 0x00;
    unsigned char servo = 0x01;
    unsigned char reply;

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    std::cout << "about to handshake" << std::endl;
    sd.write(&handshake, sizeof(char));

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
    std::cout << "sending servo command" << std::endl;
    sd.write(&servo, sizeof(char));

    std::cout << "sending servo id" << std::endl;
    sd.write((char*) 0x00, sizeof(char));
    sd.read(&reply, 1);
    std::cout << (int) reply << std::endl;

    std::cout << "sending servo angle" << std::endl;
    sd.write((char*) 0x00, sizeof(char));
    sd.read(&reply, 1);
    std::cout << (int)reply << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "sending servo command" << std::endl;
    sd.write(&servo, sizeof(char));
    // sd.read(&reply, 1);
    // std::cout << reply << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::cout << "sending servo id" << std::endl;
    sd.write((char*) 0x01, 2*sizeof(char));
    sd.read(&reply, 1);
    std::cout << reply << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::cout << "sending servo angle" << std::endl;
    sd.write((char*) 0x5A, 2*sizeof(char));
    sd.read(&reply, 1);
    std::cout << reply << std::endl;

    return 0;
}