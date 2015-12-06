#include "main_process.hpp"

#include <cstdlib>
#include <random>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "cfg.hpp"
#include "message_type.hpp"
#include "log/logger.hpp"

namespace put { namespace client {

ClientID generate_client_id() {
    std::random_device rd;
    std::uniform_int_distribution<ClientID > dist(0);
    return dist(rd);
}


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
            c_cfg->bind_address.getSockAddrPtr(),
            c_cfg -> bind_address.getAddressLen()
    );

    if (ret < 0) {
        LOG_FATAL << "Socket bind failed. " << strerror(errno);
        exit(-1);
    }

    ClientID client_id = generate_client_id();


    MsgRegReq msg_reg_req = {
            MSG_REG_REQ,
            client_id,
            c_cfg->server_send_to_address,
            c_cfg->data_block_size,
    };

    LOG_INFO << "Client id: " << msg_reg_req.client_id;
    LOG_INFO << "Server send to: " << msg_reg_req.server_send_to.toString();

    ssize_t sended_size = sendto(
            client_socket,
            reinterpret_cast<void*>(&msg_reg_req),
            sizeof(msg_reg_req),
            0,
            c_cfg -> server_address.getSockAddrPtr(),
            c_cfg -> server_address.getAddressLen()
    );

//    sockaddr_in addr;
//    addr.sin_family = PF_INET;
//    inet_pton(PF_INET, "127.0.0.1", &(addr.sin_addr));
//    addr.sin_port = htons(23334);
//    ssize_t sended_size = sendto(
//            client_socket,
//            reinterpret_cast<void*>(&msg_reg_req),
//            sizeof(msg_reg_req),
//            0,
//            reinterpret_cast<sockaddr*>(&addr),
//            sizeof(addr)
//    );

    if( sended_size < 0 || sended_size != sizeof(msg_reg_req)) {
        LOG_FATAL << "Reg msg send error. " << strerror(errno);
    }

    char buf[200];
    sockaddr_storage addr_storage;
    socklen_t sock_len = sizeof(sockaddr_storage);
    int received_size = recvfrom(
            client_socket,
            buf,
            sizeof(buf),
            0,
            reinterpret_cast<sockaddr*>(&addr_storage),
            &sock_len
    );

    LOG_INFO << "Receive size: " << received_size ;

    MsgRegRsp* msg_ptr = reinterpret_cast<MsgRegRsp*>(buf);
    LOG_INFO << "Reg Status: " << msg_ptr->reg_status;



    return 0;
}


}}

