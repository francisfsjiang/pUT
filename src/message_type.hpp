#ifndef PUT_MESSAGE_TYPE_HPP
#define PUT_MESSAGE_TYPE_HPP

namespace put {

enum MESSAGE_TYPE {
    MSG_NEW,
    MSG_DATA_REQ,
    MSG_DATA_REP,
    MSG_DATA_CHECK,
    MSG_CLOSE,
};


//close struct padding
#pragma pack(push, 1)

struct MsgCommonHeader {
    uint32_t stream_no;
    uint8_t msg_type;
    uint64_t seq_no;
};

struct MsgNew {
    MsgCommonHeader header;
    char target_file_path[128];

};

struct MsgDataReq {
    MsgCommonHeader header;
    uint32_t block_num;
    uint64_t block_pos[];
};

struct MsgDataRep {
    MsgCommonHeader header;

};


}

#endif //PUT_MESSAGE_HPP
