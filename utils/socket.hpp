#include <sys/socket.h>
#include <unistd.h>

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

    Socket(const std::string& sock_addr, Mode mode, int max_conn = 16);

    // Client
    void connect();

    // Server
    std::expected<Socket, std::system_error> next_connection();

    // Both
    std::expected<std::size_t, std::system_error> read(std::vector<std::byte>& buf);
    std::expected<std::size_t, std::system_error> write(const std::vector<std::byte>& buf);

   private:
    struct FdHolder {
        FdHolder() : fd(-1) {}
        FdHolder(int fd) : fd(fd) {}

        FdHolder(const FdHolder& holder) : fd(dup_fd(holder.fd)){};
        FdHolder& operator=(const FdHolder& rhs) {
            fd = dup_fd(rhs.fd);
            return *this;
        };

        FdHolder(FdHolder&& holder) : fd(holder.fd) { holder.fd = -1; }
        FdHolder& operator=(FdHolder&& rhs) {
            fd = rhs.fd;
            rhs.fd = -1;
            return *this;
        }

        operator int() {
            if (fd == -1) {
                throw Socket::exception("Cannot dereference invalid fd");
            }

            return fd;
        }

        FdHolder& operator=(int newfd) {
            fd = newfd;
            return *this;
        }

        ~FdHolder() {
            if (fd != -1) close(fd);
        }

        int dup_fd(int fd);

       private:
        int fd;
    };

    struct SockaddrHolder {
        SockaddrHolder() : sockaddr(std::make_unique<struct sockaddr_storage>()) {}

        SockaddrHolder(const SockaddrHolder& old) {
            size = old.size;
            auto s = *old.sockaddr;
            sockaddr = std::make_unique<struct sockaddr_storage>(s);
        }

        SockaddrHolder(SockaddrHolder&& old) : size(old.size), sockaddr(std::move(old.sockaddr)){};

        SockaddrHolder& operator=(const SockaddrHolder& rhs) {
            sockaddr = std::make_unique<struct sockaddr_storage>(*rhs.sockaddr);
            size = rhs.size;
            return *this;
        }

        SockaddrHolder& operator=(SockaddrHolder&& rhs) {
            sockaddr = std::move(rhs.sockaddr);
            size = rhs.size;
            return *this;
        };

        operator struct sockaddr *() { return reinterpret_cast<struct sockaddr*>(sockaddr.get()); }

        struct sockaddr_in* addr_in() {
            return reinterpret_cast<struct sockaddr_in*>(sockaddr.get());
        }

        struct sockaddr_un* addr_un() {
            return reinterpret_cast<struct sockaddr_un*>(sockaddr.get());
        }

        socklen_t size;

       private:
        std::unique_ptr<struct sockaddr_storage> sockaddr;
    };

    void initialize_socket_tcp(const std::string& sock_path);
    void initialize_socket_unix(const std::string& sock_path);

    FdHolder fd;
    Type type;
    Mode mode;
    bool is_listening_socket = false;  // default to the socket being "normal"

    std::string sock_path;
    SockaddrHolder my_sockaddr;
};