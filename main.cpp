
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace tlucanti
{
    class Color
    {
        static const char *r;
    };

    class SocketException : public std::exception
    {
    public:
        SocketException(const std::string &message, int error)
        {
            _message = "[r][FAIL][w] socket:[] " + message + ": [y]" + ft_strerror(error) + "[]";
        }
        const std::string &what() { return _message; }
    private:
        const char *ft_strerror(int errnum) {
            switch (errnum)
            {
                case 0: return "";
                default: return strerror(errnum);
            }
        }
        std::string _message;
    };

    class Socket
    {
    public:
        Socket(const std::string &address, int port)
        {
            struct sockaddr_in _addr = {};

            _sock = socket(AF_INET, SOCK_STREAM, 0);
            if (_sock == -1)
                throw SocketException("cannot create socket", errno);
            _addr.sin_family = AF_INET;
            _addr.sin_addr.s_addr = inet_addr(address.c_str());
            _addr.sin_port = htons(port);
            if (bind(_sock, reinterpret_cast<const sockaddr *>(&_addr), sizeof(_addr)))
                throw SocketException("cannot bind address", errno);
            if (listen(_sock, 10))
                throw SocketException("cannot listen port", errno);
        }
        Socket &operator =(const Socket &cpy)
        {
            _sock = cpy._sock;
            return *this;
        }

        Socket(int sock) : _sock(sock) {}

        [[nodiscard]] int get_sock() const { return _sock; }
    private:
        int _sock;
    };

    Socket accept(const Socket &_sock)
    {
        int int_sock = ::accept(_sock.get_sock(), nullptr, nullptr);
        if (int_sock < 0)
            throw SocketException("cannot accept socket", errno);
        return Socket(int_sock);
    }
}

int main()
{
    tlucanti::Socket sock("0.0.0.0", 8080);
    while (true)
    {
        tlucanti::Socket = tlucanti::accept(sock);
    }
}
