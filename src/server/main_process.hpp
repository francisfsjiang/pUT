#ifndef PUT_SERVER_MAIN_PROCESS_HPP
#define PUT_SERVER_MAIN_PROCESS_HPP

#include <map>

#include "inet_address.hpp"

namespace put { namespace server {


struct ClientInfo {
    unsigned long long client_id;
    InetAddress server_send_to;
    int data_block_size;
};

extern std::map<unsigned long long, ClientInfo> g_CLIENT_MAP;


int main_process();

}}

#endif //PUT_CLIENT_PROCESS_HPP
