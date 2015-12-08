#include "main_process.hpp"

#include <mutex>
#include <map>
#include <thread>
#include <vector>
#include <cstdlib>

#include <sys/socket.h>
#include <sys/fcntl.h>

#include <boost/filesystem.hpp>

#include "cfg.hpp"
#include "message_type.hpp"
#include "log/logger.hpp"


namespace put { namespace server{

const int k_THREAD_NUM = 2;

class ClientInfo {
public:
    ClientInfo(
            ClientID client_id,
            InetAddress addr,
            boost::filesystem::path file_path
    ):client_id(client_id),
      server_send_to(addr),
      file_path(file_path)
    {}
    unsigned long long client_id;
    InetAddress server_send_to;
    boost::filesystem::path file_path;
};

std::mutex g_CLIENT_MAP_LOCK;
std::map<ClientID, ClientInfo> g_CLIENT_MAP;

__thread char* g_MSG_BUFFER;

int new_client_process(
        const int& socket_fd,
        const MsgRegReq& msg,
        const sockaddr_storage* sock_addr,
        socklen_t sock_len
) {
    std::lock_guard<std::mutex> l(g_CLIENT_MAP_LOCK);

    LOG_TRACE << "Get new client, from: " << InetAddress(reinterpret_cast<const sockaddr*>(sock_addr), sock_len).toString();
    LOG_TRACE << "Client id: "            << msg.client_id;
    LOG_TRACE << "Sever sent to addr: "        << msg.server_send_to_addr;
    LOG_TRACE << "Sever sent to port: "        << msg.server_send_to_port;
    LOG_TRACE << "Request file: "         << msg.request_file;
    LOG_TRACE << "Client remain now: "    << g_CLIENT_MAP.size();

    boost::filesystem::path request_path(msg.request_file);
    boost::filesystem::path target_file_path = s_cfg->serve_path / request_path;

    MsgRegRsp msg_reg_rsp;
    msg_reg_rsp.msg_type = MSG_REG_RSP;

    ClientInfo client_info(
            msg.client_id,
            InetAddress(msg.server_send_to_addr, msg.server_send_to_port),
            target_file_path
    );

    if (!boost::filesystem::exists(target_file_path) ||
            !boost::filesystem::is_regular_file(target_file_path)) {

        msg_reg_rsp.reg_status = 0;
        strcpy(msg_reg_rsp.error_msg, "File not exists");
    }
    else if(g_CLIENT_MAP.find(msg.client_id) != g_CLIENT_MAP.end()) {
        msg_reg_rsp.reg_status = 0;
        strcpy(msg_reg_rsp.error_msg, "Client already exists");
    }
    else {
        msg_reg_rsp.reg_status = 1;
        msg_reg_rsp.file_size = boost::filesystem::file_size(target_file_path);
        g_CLIENT_MAP.insert(std::make_pair(
                msg.client_id,
                client_info
        ));
    }

    sendto(
            socket_fd,
            reinterpret_cast<void*>(&msg_reg_rsp),
            sizeof(msg_reg_rsp),
            0,
            client_info.server_send_to.getSockAddrPtr(),
            client_info.server_send_to.getAddressLen()
    );
    return 0;
}

int data_process(const int& socket_fd, MsgDataReq& msg) {
    LOG_TRACE << "Get data req: ";
    LOG_TRACE << "Client id: "          << msg.client_id;
    LOG_TRACE << "Data arr len: "       << msg.arr_len;

    g_CLIENT_MAP_LOCK.lock();
    auto result = g_CLIENT_MAP.find(msg.client_id);
    g_CLIENT_MAP_LOCK.unlock();
    if (result == g_CLIENT_MAP.end()) {
        return -1;
    }

    ClientInfo client_info = result->second;

    MsgDataRsp* msg_to_send = reinterpret_cast<MsgDataRsp*>(g_MSG_BUFFER);
    msg_to_send->msg_type = MSG_DATA_RSP;
    size_t msg_to_send_size;

    ssize_t result_size;
    off_t seek_result;

    int file_fd = open(
            client_info.file_path.c_str(),
            O_RDONLY
    );
    if (file_fd < 0) {
        LOG_ERROR << "Open file failed. " << strerror(errno);
        return -1;
    }

    size_t *block_pos_arr_ptr = msg.block_pos_arr;
    size_t *block_pos_arr_end = block_pos_arr_ptr+msg.arr_len;
    for(; block_pos_arr_ptr != block_pos_arr_end; block_pos_arr_ptr += 1) {

        msg_to_send->block_pos = *block_pos_arr_ptr;

        seek_result = lseek(file_fd,
                            *block_pos_arr_ptr,
                            SEEK_SET);
        if (seek_result != static_cast<off_t>(*block_pos_arr_ptr)) {
            LOG_ERROR << "Seek failed. " << strerror(errno);
            continue;
        }
        result_size = read(
                file_fd,
                msg_to_send->buf,
                k_DATA_BLOCK_SIZE
        );
        if (result_size < 0) {
            LOG_ERROR << "Read error. " << strerror(errno);
        }
        msg_to_send->buf_len = static_cast<size_t>(result_size);
        msg_to_send_size = sizeof(MsgDataRsp);
        result_size = sendto(
                socket_fd,
                msg_to_send,
                msg_to_send_size,
                0,
                client_info.server_send_to.getSockAddrPtr(),
                client_info.server_send_to.getAddressLen()
        );
        if (result_size < 0 || static_cast<size_t>(result_size) < msg_to_send_size) {
            LOG_ERROR << "Msg send error";
        }

    }

    close(file_fd);

    return 0;
}

//int data_check_process(const int& socket_fd, const MsgDataCheckReq& msg) {
//    LOG_TRACE << "Get data check req: ";
//    LOG_TRACE << "Client id: "          << msg.client_id;
//    return 0;
//}

int close_client_process(const MsgClientClose& msg) {
    LOG_TRACE << "Get client close: ";
    LOG_TRACE << "Client id: "          << msg.client_id;

    g_CLIENT_MAP_LOCK.lock();
    g_CLIENT_MAP.erase(msg.client_id);
    g_CLIENT_MAP_LOCK.unlock();
    return 0;
}

int thread_process(int socket_fd) {

    char* msg_buffer = new char[k_MAX_MSG_BUFFER_SIZE];
    g_MSG_BUFFER = new char[k_MAX_MSG_BUFFER_SIZE];
    sockaddr_storage sock_addr;
    socklen_t sock_addr_len = sizeof(sockaddr_storage);
    ssize_t received_size;
    Msg* msg_ptr = reinterpret_cast<Msg*>(msg_buffer);

    while (1) {

        received_size = recvfrom(
                socket_fd,
                msg_buffer,
                k_MAX_MSG_BUFFER_SIZE,
                0,
                reinterpret_cast<sockaddr*>(&sock_addr),
                &sock_addr_len
        );

        if (received_size < 0) {
            LOG_FATAL << "Receive msg error. " << strerror(errno);
            return -1;
        }

        LOG_TRACE << "Receive msg size: " << received_size ;

        switch(msg_ptr->msg_type) {
            case MSG_REG_REQ:
                new_client_process(
                        socket_fd,
                        *reinterpret_cast<MsgRegReq*>(msg_ptr),
                        &sock_addr,
                        sock_addr_len
                );
                break;
            case MSG_DATA_REQ:
                data_process(socket_fd, *reinterpret_cast<MsgDataReq*>(msg_ptr));
                break;
            case MSG_DATA_CHECK_REQ:
//                data_check_process(socket_fd, *reinterpret_cast<MsgDataCheckReq*>(msg_ptr));
                break;
            case MSG_CLIENT_CLOSE:
                close_client_process(*reinterpret_cast<MsgClientClose*>(msg_ptr));
                break;
            default:
                LOG_ERROR << "Unknown msg type. ";
                continue;
        }
    }

    delete[] msg_buffer;
    delete[] g_MSG_BUFFER;
    return 0;
}


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

    std::vector<std::thread> threads;

    for (int i = 0; i < k_THREAD_NUM; ++i) {
        threads.push_back(
                std::thread(
                        [server_socket](){thread_process(server_socket);}
                )
        );
    }

    for(auto&& iter: threads) {
        iter.join();
    }

    return 0;
}


}}

