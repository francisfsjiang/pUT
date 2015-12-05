#ifndef PUT_INET_ADDRESS_HPP
#define PUT_INET_ADDRESS_HPP

#include <string>

#include <sys/socket.h>


namespace put {

class InetAddress {
public:
    InetAddress(const char*);

    InetAddress(const char*, const in_port_t&);

    InetAddress(const in_port_t &);

    sockaddr_storage* getSockAddr();

    std::string toString();

private:

    struct sockaddr_storage address_;

    InetAddress() = delete;
};

}

#endif