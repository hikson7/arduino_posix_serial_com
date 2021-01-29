
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
