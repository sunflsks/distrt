#include <sys/socket.h>

#include <cstddef>
#include <exception>
#include <expected>
#include <memory>
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
    Socket(const std::string& sock_addr, Mode mode,
           int max_conn = 16);  // TODO: make max_conn a config file
    Socket(const Socket& sock);
    ~Socket();

    // Client
    void connect();

    // Server
    std::expected<Socket, std::system_error> next_connection();

    // Both
    std::expected<std::size_t, std::system_error> read(std::vector<std::byte>& buf);
    std::expected<std::size_t, std::system_error> write(const std::vector<std::byte>& buf);

   private:
    void initialize_socket_tcp(const std::string& sock_path);
    void initialize_socket_unix(const std::string& sock_path);

    int fd;
    Type type;
    Mode mode;
    bool is_listening_socket = false;  // default to the socket being "normal"

    std::string sock_path;
    std::unique_ptr<struct sockaddr_storage> my_sockaddr;
    socklen_t my_sockaddr_size;
};