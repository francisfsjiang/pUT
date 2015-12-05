#include "client/client_cfg.hpp"

namespace put {

ClientCfg::ClientCfg (
        const InetAddress& bind_address,
        const int&         data_block_size,
        const int&        data_check,
        const std::string& data_check_method,
        const InetAddress& server_address,
        const InetAddress& server_send_to_address
) : bind_address(bind_address),
    data_block_size(data_block_size),
    data_check(static_cast<bool>(data_check)),
    data_check_method(data_check_method),
    server_address(server_address),
    server_send_to_address(server_send_to_address) {

}

ClientCfg* c_cfg;

}
