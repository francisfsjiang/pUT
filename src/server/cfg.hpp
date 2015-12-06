#ifndef PUT_SERVER_CFG_HPP
#define PUT_SERVER_CFG_HPP

#include <map>
#include <string>

#include "inet_address.hpp"

namespace put { namespace server {

class ServerCfg {
public:
    ServerCfg (
            const InetAddress&,
            const int&,
            const int&,
            const std::string&,
            const InetAddress&,
            const InetAddress&
    );
    ServerCfg() = delete;
    ServerCfg(const ServerCfg&) = delete;
    ServerCfg& operator = (const ServerCfg&) = delete;

    InetAddress bind_address;
    int data_block_size;
    bool data_check;
    std::string data_check_method;
    InetAddress server_address;
    InetAddress server_send_to_address;

};

extern ServerCfg* c_cfg;

}}

#endif //PUT_Server_CFG_HPP
