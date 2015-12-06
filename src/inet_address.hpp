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

    sockaddr* getSockAddrPtr();

    std::string toString();

    sa_family_t getProtocolFamily();
    socklen_t getAddressLen();
    socklen_t* getAddressLenPtr();


private:

    struct sockaddr_storage address_;
    socklen_t address_len_;

    InetAddress() = delete;
};

}

#endif