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

Socket::Socket(const std::string& sock_addr, Mode mode, int num_conn) : mode(mode) {
    if (sock_addr.starts_with("tcp:")) {
        sock_path = sock_addr.substr(4);
        auto addr_in = my_sockaddr.addr_in();
        my_sockaddr.size = sizeof(struct sockaddr_in);

        auto split_loc = sock_path.find(":");
        if (split_loc == std::string::npos) {
            throw exception(std::format("Could not parse TCP socket address / path {}", sock_path));
        }

        std::string ip = sock_path.substr(0, split_loc);
        std::string port = sock_path.substr(split_loc + 1, sock_path.size());

        if (port.empty()) {
            throw exception(std::format("Could not parse socket address / path {}", sock_path));
        }

        addr_in->sin_family = AF_INET;

        try {
            addr_in->sin_port = htons(std::stoi(std::string(port)));
        } catch (std::exception& e) {
            throw exception("Could not perform conversion - " + std::string(e.what()));
        }

        if (inet_pton(AF_INET, ip.c_str(), &(addr_in->sin_addr)) <= 0) {
            throw exception("Could not process ip " + ip);
        }

        fd = socket(AF_INET, SOCK_STREAM, 0);
    }

    else if (sock_addr.starts_with("unix:")) {
        sock_path = sock_addr.substr(5);
        auto addr_un = my_sockaddr.addr_un();
        my_sockaddr.size = sizeof(sockaddr_un);

        addr_un->sun_family = AF_UNIX;
        strncpy(addr_un->sun_path, sock_path.c_str(), sock_path.size());

        fd = socket(AF_UNIX, SOCK_STREAM, 0);
    }

    else {
        auto err_str = std::format("Could not parse socket address / path {}", sock_addr);
        throw exception(err_str);
    }

    // we only need to do extra things (bind, etc) on the server-side, as well as market it as a
    // listening fd.
    if (mode == Mode::SERVER) {
        is_listening_socket = true;
        if (bind(fd, my_sockaddr, my_sockaddr.size) == -1) {
            throw exception(std::format("Could not bind socket: {}", strerror(errno)));
        }

        if (listen(fd, num_conn) == -1) {
            throw exception(std::format("Could not listen to socket: {}", strerror(errno)));
        }
    }
}

void Socket::connect() {
    if (mode != Mode::CLIENT) {
        throw exception(std::format("Cannot connect() when we are not a client!"));
    }
    if (::connect(fd, my_sockaddr, my_sockaddr.size)) {
        throw exception(std::format("Could not connect to {} - {}", sock_path, strerror(errno)));
    }
}

std::expected<Socket, std::system_error> Socket::next_connection() {
    if (!is_listening_socket) {
        throw exception("Cannot get next connection from a non_listening socket!");
    }

    Socket client_socket = *this;
    int client_fd = accept(fd, client_socket.my_sockaddr, &(client_socket.my_sockaddr.size));
    if (client_fd == -1) {
        auto err = errno;
        return std::unexpected(std::system_error(std::error_code(err, std::system_category())));
    }

    client_socket.fd = client_fd;
    client_socket.is_listening_socket = false;
    return client_socket;
}

std::expected<std::size_t, std::system_error> Socket::read(std::vector<std::byte>& buf) {
    if (is_listening_socket) {
        throw exception("Can't read on a listening socket? Call next_connection() maybe?");
    }

    int bytes_read = ::read(fd, buf.data(), buf.capacity());
    if (bytes_read == -1) {
        auto err = errno;
        return std::unexpected(std::system_error(std::error_code(err, std::system_category())));
    }

    return bytes_read;
}

std::expected<std::size_t, std::system_error> Socket::write(const std::vector<std::byte>& buf) {
    if (is_listening_socket) {
        throw exception("Can't write to a listening socket? Call accept() maybe?");
    }

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

int Socket::FdHolder::dup_fd(int fd) {
    auto newfd = dup(fd);
    if (newfd == -1) {
        auto err = errno;
        throw Socket::exception(std::format("Could not duplicate fd: {}", strerror(err)));
    }

    return newfd;
}