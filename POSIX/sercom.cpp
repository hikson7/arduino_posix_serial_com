
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "sercom.h"

SerialDescriptor::SerialDescriptor(
    const char* device_path, const int baudrate) {
    baudrate_ = baudrate;
    device_path_.append(device_path);
}

SerialDescriptor::~SerialDescriptor() {
    if (::close(fd_) < 0) {
        perror("port error");
    }
}

void SerialDescriptor::open() {
    int c_flag =    O_RDWR      // read/write operation
                |   O_NOCTTY    // not a controlling terminal
                |   O_NDELAY;   // don't delay it even if other
                                // end of the port is not up/running
    fd_ = ::open(device_path_.c_str(), c_flag);

    int old_flags;
    if (fd_ < 0) {
        perror("error opening port");
    } else {
        old_flags = fcntl(fd_, F_GETFL, 0);  // get current status
        old_flags = old_flags & ~O_NDELAY;   // turn off NDELAY
        fcntl(fd_, F_SETFL, old_flags);      // set blocking
    }
    struct termios port_option;
    memset(&port_option, 0, sizeof(port_option));

    // get current port options.
    tcgetattr(fd_, &port_option);

    /* set i/o baud rates */
    cfsetispeed(&port_option, baudrate_);
    cfsetospeed(&port_option, baudrate_);
    /* 8-bit data to be sent */
    port_option.c_cflag = (port_option.c_cflag & ~CSIZE) | CS8;
    /* prevents the port from becoming the "owner" */
    port_option.c_cflag |= (CLOCAL | CREAD);
    /* stop bit option */
    port_option.c_cflag &= ~CSTOPB;
    /* parity option */
    port_option.c_cflag &= ~(PARENB | PARODD);
    /* hardware based flow control */
    port_option.c_cflag &= ~CRTSCTS;
    /* software based flow control */
    port_option.c_iflag &= ~(IXON | IXOFF | IXANY);
    /* set cononical mode */
    port_option.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // set port options.
    tcsetattr(fd_, TCSANOW, &port_option);
}

int SerialDescriptor::read(void* reply, int buff_len) {
    int num = ::read(fd_, reply, buff_len);
    return num;
}

int SerialDescriptor::write(void* msg, int nbyte) {
    int num = ::write(fd_, msg, nbyte);
    tcdrain(fd_);
    return num;
}
