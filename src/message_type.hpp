#ifndef PUT_MESSAGE_TYPE_HPP
#define PUT_MESSAGE_TYPE_HPP

#include <cstdint>

namespace put {

enum MESSAGE_TYPE {
    MSG_DATA_REQ,
    MSG_DATA_REP,
    MSG_DATA_CHECK_REQ,
    MSG_DATA_CHECK_REP,
};


//close struct padding
#pragma pack(push, 1)

struct Msg {
    uint8_t msg_type;
    uint32_t msg_len;
    char* payload;
};

struct MsgDataReq {
    uint8_t msg_type;
    uint32_t msg_len;
    uint32_t* block_pos_arr;
};

struct MsgDataRep {
    uint8_t msg_type;
    uint32_t msg_len;
    uint32_t block_pos;
    char* buf;
};

struct MsgDataCheck {

};

struct MsgClose {

};

#pragma pack(pop)

}

#endif //PUT_MESSAGE_HPP
