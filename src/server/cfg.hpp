#ifndef PUT_SERVER_CFG_HPP
#define PUT_SERVER_CFG_HPP

#include <map>
#include <string>

#include <boost/filesystem.hpp>

#include "inet_address.hpp"

namespace put { namespace server {

class ServerCfg {
public:
    ServerCfg (
            const InetAddress&,
            const boost::filesystem::path&
    );
    ServerCfg() = delete;
    ServerCfg(const ServerCfg&) = delete;
    ServerCfg& operator = (const ServerCfg&) = delete;

    InetAddress bind_address;
    boost::filesystem::path serve_path;

};

extern ServerCfg*s_cfg;

}}

#endif //PUT_Server_CFG_HPP
