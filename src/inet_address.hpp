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

    InetAddress(const sockaddr*, socklen_t);

    const sockaddr* getSockAddrPtr() const;

    std::string toString() const;

    sa_family_t getProtocolFamily() const;
    socklen_t getAddressLen() const ;
    const socklen_t* getAddressLenPtr() const;


private:

    struct sockaddr_storage address_;
    socklen_t address_len_;

    InetAddress() = delete;
};

}

#endif