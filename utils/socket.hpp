#include <sys/socket.h>

#include <cstddef>
#include <exception>
#include <expected>
#include <string>
#include <system_error>
#include <vector>

class Socket {
   public:
    enum Type { TCP, UNIX };
    enum class Mode { SERVER, CLIENT };

    class exception : std::runtime_error {
       public:
        exception(const std::string& e) : std::runtime_error("Error initializing socket - " + e) {}
    };

    Socket() = delete;

    Socket(const std::string& sock_addr, Mode mode);
    ~Socket();

    // Client
    void connect();

    // Server

    std::expected<std::size_t, std::system_error> read(std::vector<std::byte>& buf);
    std::expected<std::size_t, std::system_error> write(const std::vector<std::byte>& buf);

   private:
    void initialize_socket_tcp(const std::string& sock_path);
    void initialize_socket_unix(const std::string& sock_path);

    int fd;
    Type type;
    Mode mode;
    std::string sock_path;
    struct sockaddr my_sockaddr = {};
};