#include "main_process.hpp"

#include <cstdlib>
#include <sys/socket.h>

#include "cfg.hpp"
#include "message_type.hpp"
#include "log/logger.hpp"

namespace put { namespace server{

std::map<unsigned long long, ClientInfo> g_CLIENT_MAP;

int main_process() {

    int server_socket = socket(
            s_cfg->bind_address.getProtocolFamily(),
            SOCK_DGRAM,
            0
    );
    if (server_socket < 0) {
        LOG_FATAL << "Create socket failed. " << strerror(errno);
        exit(-1);
    }

    int ret = bind(
            server_socket,
            s_cfg->bind_address.getSockAddrPtr(),
            s_cfg-> bind_address.getAddressLen()
    );
    if (ret < 0) {
        LOG_FATAL << "Socket bind failed. " << strerror(errno);
        exit(-1);
    }

//    ret = listen(server_socket, 2000);
//    if (ret < 0) {
//        LOG_FATAL << "Socket listen failed. " << strerror(errno);
//        exit(-1);
//    }

    ssize_t received_size;
    char buf[2000];

    sockaddr_storage addr_storage;
    socklen_t sock_len = sizeof(sockaddr_storage);

    while (1) {
        received_size = recvfrom(
                server_socket,
                buf,
                sizeof(buf),
                0,
                reinterpret_cast<sockaddr*>(&addr_storage),
                &sock_len
        );

        LOG_INFO << "Receive: " << buf ;

        MsgRegReq* msg_ptr = reinterpret_cast<MsgRegReq*>(buf);
        LOG_INFO << "Client id: " << msg_ptr->client_id;
        LOG_INFO << "Data_block_size: " << msg_ptr->data_block_size;
        LOG_INFO << "Sever_sent_tp" << msg_ptr->server_send_to.toString();


        MsgRegRsp msg_reg_rsp;
        msg_reg_rsp.msg_type = MSG_REG_RSP;
        msg_reg_rsp.reg_status = 1;

        sendto(
                server_socket,
                reinterpret_cast<void*>(&msg_reg_rsp),
                sizeof(msg_reg_rsp),
                0,
                msg_ptr->server_send_to.getSockAddrPtr(),
                msg_ptr->server_send_to.getAddressLen()
        );

    }


    return 0;
}


}}

