#include "inet_address.hpp"

#include <cstring>

#include <arpa/inet.h>
#include <sys/un.h>

namespace put {


InetAddress::InetAddress(const char *addr) {
    sockaddr_un* un_ptr = reinterpret_cast<sockaddr_un*>(&address_);
    un_ptr -> sun_len  = sizeof(sockaddr_un);
    un_ptr -> sun_family = PF_UNIX;
    std::strcpy(un_ptr -> sun_path, addr);
}

InetAddress::InetAddress(const char* addr, const in_port_t& port) {
    int ret = inet_pton(PF_INET, addr, &address_);
    if (ret == 0) {
        ret = inet_pton(PF_INET6, addr, &address_);
        if (ret == 0) {
            throw std::invalid_argument(addr);
        }
        else {
            sockaddr_in6 *in6_ptr = reinterpret_cast<sockaddr_in6*>(&address_);
            in6_ptr -> sin6_family = PF_INET6;
            in6_ptr -> sin6_addr = in6addr_any;
            in6_ptr -> sin6_port = port;
            in6_ptr -> sin6_len= sizeof(sockaddr_in6);
        }
    }
    else {
        sockaddr_in *in_ptr = reinterpret_cast<sockaddr_in*>(&address_);
        in_ptr -> sin_family = PF_INET;
        in_ptr -> sin_addr.s_addr = INADDR_ANY;
        in_ptr -> sin_port = port;
        in_ptr -> sin_len = sizeof(sockaddr_in);
    }
}

InetAddress::InetAddress(const in_port_t& port) {
    sockaddr_in6 *in6_ptr = reinterpret_cast<sockaddr_in6*>(&address_);
    in6_ptr -> sin6_family = PF_INET6;
    in6_ptr -> sin6_addr = in6addr_any;
    in6_ptr -> sin6_port = port;
    in6_ptr -> sin6_len = sizeof(sockaddr_in6);

}

sockaddr_storage* InetAddress::getSockAddr() {
    return &address_;
}

std::string InetAddress::toString() {
    char addr_str_buf[INET6_ADDRSTRLEN];
    inet_ntop(address_.ss_family, &address_, addr_str_buf, sizeof(addr_str_buf));
    return std::string(addr_str_buf);
}

}