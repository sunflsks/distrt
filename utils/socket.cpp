#include "socket.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <exception>
#include <filesystem>
#include <format>

Socket::Socket(const std::string& sock_addr, Mode mode) : mode(mode) {
    if (sock_addr.starts_with("tcp:")) {
        sock_path = sock_addr.substr(4);
        initialize_socket_tcp(sock_path);
    }

    else if (sock_addr.starts_with("unix:")) {
        sock_path = sock_addr.substr(5);
        initialize_socket_unix(sock_path);
    }

    else {
        auto err_str = std::format("Could not parse socket address / path {}", sock_addr);
        throw exception(err_str);
    }
}

void Socket::initialize_socket_tcp(const std::string& sock_path) {
    struct sockaddr_in& addr_in = reinterpret_cast<struct sockaddr_in&>(my_sockaddr);

    auto split_loc = sock_path.find(":");
    if (split_loc == std::string::npos) {
        throw exception(std::format("Could not parse TCP socket address / path {}", sock_path));
    }

    std::string ip = sock_path.substr(0, split_loc);
    std::string port = sock_path.substr(split_loc + 1, sock_path.size());

    if (port.empty()) {
        throw exception(std::format("Could not parse socket address / path {}", sock_path));
    }

    addr_in.sin_family = AF_INET;

    try {
        addr_in.sin_port = htons(std::stoi(std::string(port)));
    } catch (std::exception& e) {
        throw exception("Could not perform conversion - " + std::string(e.what()));
    }

    if (inet_pton(AF_INET, ip.c_str(), &addr_in.sin_addr) <= 0) {
        throw exception("Could not process ip " + ip);
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);
}

void Socket::initialize_socket_unix(const std::string& sock_path) {
    struct sockaddr_un& addr_un = reinterpret_cast<struct sockaddr_un&>(my_sockaddr);

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (!std::filesystem::exists(sock_path)) {
        throw exception(std::format(
            "Could not initialize / connect to socket at path {} - does not exist", sock_path));
    }

    addr_un.sun_family = AF_UNIX;
    strncpy(addr_un.sun_path, sock_path.c_str(), sock_path.size());
}

void Socket::connect() {
    if (::connect(fd, &my_sockaddr, sizeof(my_sockaddr))) {
        throw exception(std::format("Could not connect to {} - {}", sock_path, strerror(errno)));
    }
}

std::expected<std::size_t, std::system_error> Socket::read(std::vector<std::byte>& buf) {
    int bytes_read = ::read(fd, buf.data(), buf.capacity());
    if (bytes_read == -1) {
        auto err = errno;
        return std::unexpected(std::system_error(std::error_code(err, std::system_category())));
    }

    return bytes_read;
}

std::expected<std::size_t, std::system_error> Socket::write(const std::vector<std::byte>& buf) {
    auto buf_ptr = buf.data();
    std::size_t sent_data = 0;
    while (sent_data < buf.size()) {
        auto err = ::send(fd, buf.data() + sent_data, buf.size() - sent_data, 0);
        if (err == -1) {
            auto err = errno;
            return std::unexpected(std::system_error(std::error_code(err, std::system_category())));
        }

        sent_data += err;
    }

    return sent_data;
}

Socket::~Socket() {
    close(fd);
}