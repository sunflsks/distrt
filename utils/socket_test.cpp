#include <stdio.h>

#include <iostream>

#include "socket.hpp"

int main(void) {
    Socket new_sock = Socket("unix:/tmp/delme.sock", Socket::Mode::SERVER);

    while (auto sock = new_sock.next_connection()) {
        std::vector<std::byte> buf(2048);
        auto size = sock->read(buf);
        auto bs = std::string(reinterpret_cast<const char*>(buf.data()), *size);
        std::cout << bs << std::endl;
    }
}
