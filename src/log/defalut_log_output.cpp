#include "log/defalut_log_output.hpp"

#include <cstring>
#include <iostream>

namespace put { namespace log {

int DefaultLogOutput::append(const char* buf, size_t len) {
    while(1) {
        if (len <= k_BUFFER_SIZE) {
            memcpy(buffer_, buf, len);
            buffer_[len] = '\0';
            std::cout << buffer_;
            return 0;
        }
        else {
            memcpy(buffer_, buf, k_BUFFER_SIZE);
            buffer_[k_BUFFER_SIZE] = '\0';
            std::cout << buffer_;
            len -= k_BUFFER_SIZE;
        }
    }
}

int DefaultLogOutput::flush() {
    return 0;
}

DefaultLogOutput::~DefaultLogOutput() {

}


}}
