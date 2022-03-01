#ifndef BOT_HPP
# define BOT_HPP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>



#define BUF_SIZE 512
const std::string   toUpper( const std::string &str );
class Bot
{
private:
    int             _sockfd;
    const char*     _hostname;
    const char*     _port;
    const char*     _nick;
    struct addrinfo hints, *servinfo, *p;
    char            s[INET6_ADDRSTRLEN];
    std::string     _delimeter;
    std::string     _pass;
    std::string     _buffer;
    std::string     _recvBuffer;
    std::string     _sendBuffer;

public:
    Bot( const char* host, const char* port, const char* bname, const char* pass);
    void    run  ();
private:
    bool    _recv(       std::string &status);
    bool    _send( const std::string &status);
    std::string    _kick(const std::string &name, const std::string &chan) const;
    std::string    _ban(const std::string &name, const std::string &chan) const;
    std::string    _printMsg (const std::string &name, const char *fname) const;
};

#endif /* CLIENT_HPP */
