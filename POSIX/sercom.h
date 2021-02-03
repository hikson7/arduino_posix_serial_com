/**
 * @file sercom.h
 * @author Hikari Hashida
 * @brief Serial communication class for POSIX based system and a microcontroller (tested on Arduino).
 * @version 0.1
 * @date 2021-01-31
 * 
 */
#include <string>

class SerialDescriptor {
 public:
    SerialDescriptor(const char* device_path, const int baudrate);
    ~SerialDescriptor();

    void open();
    int read(void* reply, int buff_len);
    int write(void* msg, int nbyte);

 private:
    int fd_;
    std::string device_path_;
    int baudrate_;
};
