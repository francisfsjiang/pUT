#ifndef PUT_MESSAGE_TYPE_HPP
#define PUT_MESSAGE_TYPE_HPP

#include <cstdint>

#include "inet_address.hpp"

namespace put {

enum MESSAGE_TYPE {
    MSG_REG_REQ         = 0x40,
    MSG_REG_RSP         = 0x41,
    MSG_CLIENT_CLOSE    = 0x42,
    MSG_DATA_REQ        = 0x43,
    MSG_DATA_RSP        = 0x44,
    MSG_DATA_CHECK_REQ  = 0x45,
    MSG_DATA_CHECK_RSP  = 0x46,
};

typedef unsigned long long ClientID;

const int k_MAX_MSG_BUFFER_SIZE = 2000;

const size_t k_DATA_BLOCK_SIZE = 1024;
const int k_MAX_DATA_REQ_ARR_LEN = 125;
//close struct padding

struct Msg {
    uint8_t msg_type;
};

struct MsgRegReq {
    uint8_t msg_type;
    ClientID client_id;
    char server_send_to_addr[k_MAX_INET_ADDRESS_LEN];
    in_port_t server_send_to_port;
    char request_file[128];
};

struct MsgRegRsp {
    uint8_t msg_type;
    bool reg_status;
    size_t file_size;
    char error_msg[128];

};

struct MsgClientClose{
    uint8_t msg_type;
    ClientID client_id;
};

struct MsgDataReq {
    uint8_t msg_type;
    ClientID client_id;
    uint32_t seq_id;
    size_t arr_len;
    size_t block_pos_arr[k_MAX_DATA_REQ_ARR_LEN];
};

struct MsgDataRsp {
    uint8_t msg_type;
    size_t block_pos;
    size_t buf_len;
    char buf[k_DATA_BLOCK_SIZE];
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
