#ifndef PUT_MESSAGE_TYPE_HPP
#define PUT_MESSAGE_TYPE_HPP

#include <cstdint>

#include "inet_address.hpp"

namespace put {

enum MESSAGE_TYPE {
    MSG_REG_REQ,
    MSG_REG_RSP,
    MSG_CLIENT_CLOSE,
    MSG_DATA_REQ,
    MSG_DATA_RSP,
    MSG_DATA_CHECK_REQ,
    MSG_DATA_CHECK_RSP,
};

typedef unsigned long long ClientID;

const int k_MAX_MSG_BUFFER_SIZE = 2000;

const int k_MAX_DATA_REQ_ARR_LEN = 250;
//close struct padding

struct Msg {
    uint8_t msg_type;
};

struct MsgRegReq {
    uint8_t msg_type;
    ClientID client_id;
    InetAddress server_send_to;
    size_t data_block_size;
    std::string request_file;
};

struct MsgRegRsp {
    uint8_t msg_type;
    bool reg_status;
    size_t file_size;
    std::string error_msg;

};

struct MsgClientClose{
    uint8_t msg_type;
    ClientID client_id;
};

struct MsgDataReq {
    uint8_t msg_type;
    ClientID client_id;
    size_t arr_len;
    size_t* block_pos_arr;
};

struct MsgDataRsp {
    uint8_t msg_type;
    size_t block_pos;
    size_t buf_len;
    char* buf;
};

struct MsgDataCheckReq {
    uint8_t msg_type;
    ClientID client_id;

};

struct MsgDataCheckRsp {
    uint8_t msg_type;
};

}

#endif //PUT_MESSAGE_HPP
