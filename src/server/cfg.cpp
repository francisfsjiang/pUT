#include "cfg.hpp"

namespace put { namespace server {

ServerCfg::ServerCfg (
        const InetAddress& bind_address,
        const boost::filesystem::path& serve_path
) : bind_address(bind_address),
    serve_path(serve_path) {
}

ServerCfg*s_cfg;

}}
