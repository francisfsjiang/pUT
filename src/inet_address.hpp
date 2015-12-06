#ifndef PUT_INET_ADDRESS_HPP
#define PUT_INET_ADDRESS_HPP

#include <string>

#include <netinet/in.h>
#include <sys/socket.h>


namespace put {

class InetAddress {
public:
    InetAddress(const char*);

    InetAddress(const char*, const in_port_t&);

    InetAddress(const in_port_t &);

    sockaddr* getSockAddr();

    std::string toString();

    sa_family_t getProtocolFamily();
    uint32_t getAddressLen();

private:

    struct sockaddr_storage address_;
    uint32_t address_len_;

    InetAddress() = delete;
};

}

#endif