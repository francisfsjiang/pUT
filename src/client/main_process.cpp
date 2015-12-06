#include "main_process.hpp"

#include <cstdlib>
#include <sys/socket.h>

#include "cfg.hpp"
#include "message_type.hpp"
#include "log/logger.hpp"

namespace put { namespace client {


int client_main_process() {

    int client_socket = socket(
            c_cfg->bind_address.getProtocolFamily(),
            SOCK_DGRAM,
            0
    );

    if (client_socket < 0) {
        LOG_FATAL << "Create socket failed. " << strerror(errno);
        exit(-1);
    }

    int ret = bind(
            client_socket,
            c_cfg -> bind_address.getSockAddr(),
            c_cfg -> bind_address.getAddressLen()
    );

    if (ret < 0) {
        LOG_FATAL << "Socket bind failed. " << strerror(errno);
        exit(-1);
    }
    return 0;
}


}}

