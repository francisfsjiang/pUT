#include "inet_address.hpp"

#include <stdexcept>
#include <cstring>

#include <arpa/inet.h>
#include <sys/un.h>

namespace put {


InetAddress::InetAddress(const char *addr) {
    sockaddr_un* un_ptr = reinterpret_cast<sockaddr_un*>(&address_);
    un_ptr -> sun_family = PF_UNIX;
    std::strcpy(un_ptr -> sun_path, addr);
    address_len_ = sizeof(sockaddr_un);
}

InetAddress::InetAddress(const char* addr, const in_port_t& port) {
    sockaddr_in *in_ptr = reinterpret_cast<sockaddr_in*>(&address_);
    int ret = inet_pton(PF_INET, addr, &in_ptr->sin_addr.s_addr);
    if (ret == 0) {
        sockaddr_in6 *in6_ptr = reinterpret_cast<sockaddr_in6*>(&address_);
        ret = inet_pton(PF_INET6, addr, &in6_ptr->sin6_addr);
        if (ret == 0) {
            throw std::invalid_argument(addr);
        }
        else {
            in6_ptr -> sin6_family = PF_INET6;
            in6_ptr -> sin6_addr = in6addr_any;
            in6_ptr -> sin6_port = htons(port);
            address_len_ = sizeof(sockaddr_in6);
        }
    }
    else {
        in_ptr -> sin_family = PF_INET;
        in_ptr -> sin_addr.s_addr = INADDR_ANY;
        in_ptr -> sin_port = htons(port);
        address_len_ = sizeof(sockaddr_in);
    }
}

InetAddress::InetAddress(const in_port_t& port) {
    sockaddr_in6 *in6_ptr = reinterpret_cast<sockaddr_in6*>(&address_);
    in6_ptr -> sin6_family = PF_INET6;
    in6_ptr -> sin6_addr = in6addr_any;
    in6_ptr -> sin6_port = port;
    address_len_ = sizeof(sockaddr_in6);

}

sockaddr* InetAddress::getSockAddrPtr() {
    return reinterpret_cast<sockaddr*>(&address_);
}

std::string InetAddress::toString() {
    char addr_str_buf[INET6_ADDRSTRLEN];
    inet_ntop(address_.ss_family, &address_, addr_str_buf, sizeof(addr_str_buf));
    return std::string(addr_str_buf);
}

sa_family_t InetAddress::getProtocolFamily() {
    return reinterpret_cast<sockaddr_in6*>(&address_) -> sin6_family;
}

socklen_t InetAddress::getAddressLen() {
    return address_len_;
}

socklen_t * InetAddress::getAddressLenPtr() {
    return &address_len_;
}

}