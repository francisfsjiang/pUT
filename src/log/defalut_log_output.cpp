#include "log/defalut_log_output.hpp"

#include <cstring>
#include <iostream>

namespace put { namespace log {

int DefaultLogOutput::append(const char* buf, size_t len) {
    if (len <= avail_) {
        memcpy(cur_, buf, len);
        cur_ += len;
        avail_ -= len;
        return 0;
    }
    else {
        flush();
        return append(buf, len);
    }
}

int DefaultLogOutput::add(size_t size) {
    cur_ += size;
    avail_ -= size;
    return size;
}

int DefaultLogOutput::flush() {
    *cur_ = '\0';
    std::cout << buffer_;
    cur_ = buffer_;
    avail_ = k_BUFFER_SIZE;
    return 0;
}

char* DefaultLogOutput::buf() {
    return cur_;
}

size_t DefaultLogOutput::avail() {
    return avail_;
};

DefaultLogOutput::~DefaultLogOutput() {

}


}}
