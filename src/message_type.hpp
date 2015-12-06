#ifndef PUT_MESSAGE_TYPE_HPP
#define PUT_MESSAGE_TYPE_HPP

#include <cstdint>

#include "inet_address.hpp"

namespace put {

enum MESSAGE_TYPE {
    MSG_REG_REQ,
    MSG_REG_RSP,
    MSG_DATA_REQ,
    MSG_DATA_RSP,
    MSG_DATA_CHECK_REQ,
    MSG_DATA_CHECK_RSP,
};

typedef unsigned long long ClientID;


//close struct padding
#pragma pack(push, 1)

struct Msg {
    uint8_t msg_type;
    char* payload;
};

struct MsgRegReq {
    uint8_t msg_type;
    ClientID client_id;
    InetAddress server_send_to;
    int data_block_size;
};

struct MsgRegRsp {
    uint8_t msg_type;
    bool reg_status;
};

struct MsgDataReq {
    uint8_t msg_type;
    uint32_t arr_len;
    uint32_t* block_pos_arr;
};

struct MsgDataRsp {
    uint8_t msg_type;
    uint32_t block_pos;
    uint32_t buf_len;
    char* buf;
};

struct MsgDataCheckReq {

};

struct MsgDataCheckRsp {

};

#pragma pack(pop)

}

#endif //PUT_MESSAGE_HPP
