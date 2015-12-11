#include "inet_address.hpp"

#include <stdexcept>
#include <cstring>

#include <arpa/inet.h>
#include <sys/un.h>

namespace put {

const int k_MAX_INET_PORT_LEN = 6;

InetAddress::InetAddress(const char *addr) {
    memset(&address_, 0, sizeof(address_));
    sockaddr_un* un_ptr = reinterpret_cast<sockaddr_un*>(&address_);
    un_ptr -> sun_family = PF_UNIX;
    std::strcpy(un_ptr -> sun_path, addr);
    address_len_ = sizeof(sockaddr_un);
}

InetAddress::InetAddress(const char* addr, const in_port_t& port) {
    memset(&address_, 0, sizeof(address_));
    sockaddr_in *in_ptr = reinterpret_cast<sockaddr_in*>(&address_);
    int ret = inet_pton(PF_INET, addr, &in_ptr->sin_addr.s_addr);
    if (ret != 1) {
        sockaddr_in6 *in6_ptr = reinterpret_cast<sockaddr_in6*>(&address_);
        ret = inet_pton(PF_INET6, addr, &in6_ptr->sin6_addr);
        if (ret != 1) {
            throw std::invalid_argument(addr);
        }
        else {
            in6_ptr -> sin6_family = PF_INET6;
            in6_ptr -> sin6_port = htons(port);
            address_len_ = sizeof(sockaddr_in6);
        }
    }
    else {
        in_ptr -> sin_family = PF_INET;
        in_ptr -> sin_port = htons(port);
        address_len_ = sizeof(sockaddr_in);
    }
}

InetAddress::InetAddress(const in_port_t& port) {
    memset(&address_, 0, sizeof(address_));
    sockaddr_in6 *in6_ptr = reinterpret_cast<sockaddr_in6*>(&address_);
    in6_ptr -> sin6_family = PF_INET6;
    in6_ptr -> sin6_addr = in6addr_any;
    in6_ptr -> sin6_port = port;
    address_len_ = sizeof(sockaddr_in6);

}

InetAddress::InetAddress(const sockaddr* sock_addr, socklen_t sock_len) {
    memcpy(&address_, sock_addr, sock_len);
    address_len_ = sock_len;
}

const sockaddr* InetAddress::getSockAddrPtr() const{
    return reinterpret_cast<const sockaddr*>(&address_);
}

std::string InetAddress::toString() const{
    char buf[k_MAX_INET_ADDRESS_LEN + k_MAX_INET_PORT_LEN];
    if (address_.ss_family == PF_INET) {
        const sockaddr_in *in_ptr = reinterpret_cast<const sockaddr_in*>(&address_);
        inet_ntop(address_.ss_family, &in_ptr->sin_addr, buf, sizeof(buf));
        sprintf(buf + k_MAX_INET_ADDRESS_LEN, ":%d", ntohs(in_ptr->sin_port));
    }
    else {
        const sockaddr_in6 *in6_ptr = reinterpret_cast<const sockaddr_in6*>(&address_);
        inet_ntop(address_.ss_family, &in6_ptr->sin6_addr, buf, sizeof(buf));
        sprintf(buf + k_MAX_INET_ADDRESS_LEN, ".%d", ntohs(in6_ptr->sin6_port));
    }
    return std::string(buf) + std::string(buf+k_MAX_INET_ADDRESS_LEN);
}

std::string InetAddress::getAddrString() const{
    char buf[k_MAX_INET_ADDRESS_LEN];
    if (address_.ss_family == PF_INET) {
        const sockaddr_in *in_ptr = reinterpret_cast<const sockaddr_in*>(&address_);
        inet_ntop(address_.ss_family, &in_ptr->sin_addr, buf, sizeof(buf));
    }
    else {
        const sockaddr_in6 *in6_ptr = reinterpret_cast<const sockaddr_in6*>(&address_);
        inet_ntop(address_.ss_family, &in6_ptr->sin6_addr, buf, sizeof(buf));
    }
    return std::string(buf);
}

in_port_t InetAddress::getPort() const {
    if (address_.ss_family == PF_INET) {
        const sockaddr_in *in_ptr = reinterpret_cast<const sockaddr_in*>(&address_);
        return ntohs(in_ptr->sin_port);
    }
    else {
        const sockaddr_in6 *in6_ptr = reinterpret_cast<const sockaddr_in6*>(&address_);
        return ntohs(in6_ptr->sin6_port);
    }
}

sa_family_t InetAddress::getProtocolFamily() const{
    return reinterpret_cast<const sockaddr_in6*>(&address_) -> sin6_family;
}

socklen_t InetAddress::getAddressLen() const{
    return address_len_;
}

const socklen_t * InetAddress::getAddressLenPtr() const{
    return &address_len_;
}

}