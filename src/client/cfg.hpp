#ifndef PUT_CLIENT_CFG_HPP
#define PUT_CLIENT_CFG_HPP

#include <map>
#include <string>

#include "inet_address.hpp"

namespace put { namespace client {

class ClientCfg {
public:
    ClientCfg (
            const InetAddress&,
            const int&,
            const int&,
            const std::string&,
            const InetAddress&,
            const InetAddress&
    );
    ClientCfg() = delete;
    ClientCfg(const ClientCfg&) = delete;
    ClientCfg& operator = (const ClientCfg&) = delete;

    InetAddress bind_address;
    int data_block_size;
    bool data_check;
    std::string data_check_method;
    InetAddress server_address;
    InetAddress server_send_to_address;

};

extern ClientCfg* c_cfg;

}}

#endif //PUT_CLIENT_CFG_HPP
