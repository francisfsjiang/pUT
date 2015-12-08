#include "main_process.hpp"

#include <random>
#include <set>
#include <thread>
#include <mutex>
#include <cstdlib>

#include <sys/socket.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>

#include <boost/filesystem.hpp>

#include "cfg.hpp"
#include "message_type.hpp"
#include "log/logger.hpp"

namespace put { namespace client {

__thread char* g_MSG_BUFFER;

const int k_RECV_THREAD_NUM = 1;

ClientID g_CLIENT_ID;

boost::filesystem::path g_FILE_PATH;

std::mutex g_FILE_BLOCKS_SET_LOCK;
std::set<size_t> g_FILE_BLOCKS_SET;
std::mutex g_FILE_LOCK;
int g_FILE_FD;

ClientID generate_client_id() {
    std::random_device rd;
    std::uniform_int_distribution<ClientID > dist(0);
    return dist(rd);
}

int send_data_req(const int& socket_fd, const MsgDataReq* msg_data_req) {
    size_t msg_data_req_size = sizeof(MsgDataReq) + msg_data_req->arr_len * sizeof(size_t) - sizeof(size_t*);
    ssize_t send_result;
    send_result = sendto(
            socket_fd,
            msg_data_req,
            msg_data_req_size,
            0,
            c_cfg->server_address.getSockAddrPtr(),
            c_cfg->server_address.getAddressLen()
    );

    if (send_result < 0) {
        LOG_ERROR << "Msg data req send error." << strerror(errno);
        return -1;
    }

    LOG_TRACE << "Msg data req sent, arr len: " << msg_data_req->arr_len;
    return 0;
}

int data_request_process(const int& socket_fd) {
    g_MSG_BUFFER = new char[k_MAX_MSG_BUFFER_SIZE];
    std::set<size_t> blocks;
    MsgDataReq* msg_data_req = reinterpret_cast<MsgDataReq*>(g_MSG_BUFFER);
    msg_data_req->msg_type = MSG_DATA_REQ;
    msg_data_req->client_id = g_CLIENT_ID;
    size_t* arr_ptr = reinterpret_cast<size_t*>(&msg_data_req->block_pos_arr);
    size_t arr_len;

    while (1) {
        {
            std::lock_guard<std::mutex> l(g_FILE_BLOCKS_SET_LOCK);
            if (g_FILE_BLOCKS_SET.size() == 0) {
                break;
            }
            blocks = g_FILE_BLOCKS_SET;
        }
        arr_len = 0;
        for(auto&& block: blocks) {
            if(arr_len >= k_MAX_DATA_REQ_ARR_LEN) {
                msg_data_req->arr_len = arr_len;
                send_data_req(socket_fd, msg_data_req);
                arr_len = 0;
            }

            arr_ptr[arr_len] = block;
            ++arr_len;
        }
        if(arr_len > 0) {
            msg_data_req->arr_len = arr_len;
            send_data_req(socket_fd, msg_data_req);
        }
        break;

    }

    return 0;
}


int receive_data_process(MsgDataRsp& msg) {

    LOG_TRACE << "Get data rsp: ";
    LOG_TRACE << "Block pos: "        << msg.block_pos;
    LOG_TRACE << "Buffer len: "       << msg.buf_len;

    {
        std::lock_guard<std::mutex> l(g_FILE_BLOCKS_SET_LOCK);
        g_FILE_BLOCKS_SET.erase(msg.block_pos);
    }

    {
        char* msg_buf_ptr = reinterpret_cast<char*>(&msg.buf);
        std::lock_guard<std::mutex> l(g_FILE_LOCK);
        off_t seek_result = lseek(
                g_FILE_FD,
                msg.block_pos,
                SEEK_SET
        );
        if (seek_result != static_cast<off_t>(msg.block_pos)) {
            LOG_ERROR << "Seek failed. " << strerror(errno);
            return -1;
        }
        ssize_t result_size = write(
                g_FILE_FD,
                msg_buf_ptr,
                msg.buf_len
        );
        if (result_size < 0) {
            LOG_ERROR << "Write error. " << strerror(errno);
        }
    }

    return 0;
}



int receive_thread_process(const int& socket_fd) {

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
            case MSG_DATA_RSP:
                receive_data_process(
                        *reinterpret_cast<MsgDataRsp*>(msg_ptr)
                );
                break;
            case MSG_DATA_CHECK_RSP:
//                data_check_process(socket_fd, *reinterpret_cast<MsgDataCheckReq*>(msg_ptr));
                break;
            case MSG_REG_RSP:
                break;
            default:
                LOG_ERROR<< "Unknown msg type. ";
                continue;
        }
    }

    delete[] msg_buffer;
    delete[] g_MSG_BUFFER;
    return 0;
    return 0;
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
    int ret;
//    int ret = bind(
//            client_socket,
//            c_cfg->bind_address.getSockAddrPtr(),
//            c_cfg -> bind_address.getAddressLen()
//    );
//    if (ret < 0) {
//        LOG_FATAL << "Socket bind failed. " << strerror(errno);
//        exit(-1);
//    }

    g_CLIENT_ID = generate_client_id();

    g_MSG_BUFFER = new char[k_MAX_MSG_BUFFER_SIZE];
    MsgRegReq* msg_reg_req = reinterpret_cast<MsgRegReq*>(g_MSG_BUFFER);
    msg_reg_req->msg_type = MSG_REG_REQ;
    msg_reg_req->client_id = g_CLIENT_ID;
    strcpy(msg_reg_req->server_send_to_addr, c_cfg->server_send_to_address.getAddrString().c_str());
    msg_reg_req->server_send_to_port = c_cfg->server_send_to_address.getPort();
    strcpy(msg_reg_req->request_file, c_cfg->request_file_path.c_str());

    LOG_INFO << "Client id: " << msg_reg_req->client_id;
    LOG_INFO << "Server send to addr: " << msg_reg_req->server_send_to_addr;
    LOG_INFO << "Server send to port: " << msg_reg_req->server_send_to_port;

    ssize_t sended_size = sendto(
            client_socket,
            msg_reg_req,
            sizeof(MsgRegReq),
            0,
            c_cfg -> server_address.getSockAddrPtr(),
            c_cfg -> server_address.getAddressLen()
    );
    if( sended_size < 0 || sended_size != sizeof(msg_reg_req)) {
        LOG_FATAL << "Reg msg send error. " << strerror(errno);
    }

    char buf[200];
    sockaddr_storage addr_storage;
    socklen_t sock_len = sizeof(sockaddr_storage);
    ssize_t received_size = recvfrom(
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
    if (msg_ptr->reg_status != 1) {
        LOG_FATAL << "Error msg: " << msg_ptr->error_msg;
        exit(-1);
    }

    LOG_INFO << "File size: " << msg_ptr->file_size;

    g_FILE_PATH = boost::filesystem::path(c_cfg->request_file_path);
    if (boost::filesystem::exists(g_FILE_PATH)) {
        LOG_INFO << "File already exists.";
        exit(0);
    }
    g_FILE_FD = open(
            g_FILE_PATH.c_str(),
            O_CREAT|O_WRONLY
    );
    ret = ftruncate(g_FILE_FD, msg_ptr->file_size);

    for (size_t pos = 0; pos < msg_ptr->file_size; pos += k_DATA_BLOCK_SIZE) {
        g_FILE_BLOCKS_SET.insert(pos);
    }

    std::thread data_req_thread(
            [client_socket](){
                data_request_process(client_socket);
            }
    );

    std::vector<std::thread> threads;

    for (int i = 0; i < k_RECV_THREAD_NUM; ++i) {
        threads.push_back(
                std::thread(
                        [client_socket](){
                            receive_thread_process(client_socket);
                        }
                )
        );
    }

    for(auto&& iter: threads) {
        iter.join();
    }

    data_req_thread.join();

    return 0;
}


}}

