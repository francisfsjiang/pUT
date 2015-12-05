#include "log/log_stream.hpp"

#include <cstring>

namespace put { namespace log {

LogStream::LogStream(LogOutput* output)
        : output_(output){
}

LogStream::~LogStream() {
    output_ -> append("\n", 1);
}

int LogStream::write_bytes(const char *buf, size_t len) {
    return output_ -> append(buf, len);
}

LogStream& LogStream::operator<<(const char * str) {
    size_t len = strlen(str);
    output_ -> append(str, len);
    return *this;
}

LogStream& LogStream::operator<<(const int& v) {
    char buf[11];
    snprintf(buf, sizeof(buf), "%d", v);
    output_ ->append(buf, strlen(buf));
    return *this;
}

}}
