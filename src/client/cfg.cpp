#include "cfg.hpp"

namespace put { namespace client {

ClientCfg::ClientCfg (
        const InetAddress& bind_address,
        const size_t&         data_block_size,
        const int&         data_check,
        const std::string& data_check_method,
        const InetAddress& server_address,
        const InetAddress& server_send_to_address,
        const std::string& request_file_path
) : bind_address(bind_address),
    data_block_size(data_block_size),
    data_check(static_cast<bool>(data_check)),
    data_check_method(data_check_method),
    server_address(server_address),
    server_send_to_address(server_send_to_address),
    request_file_path(request_file_path){

}

ClientCfg* c_cfg;

}}
