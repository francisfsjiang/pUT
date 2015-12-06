#ifndef PUT_LOG_LOG_STREAM_HPP
#define PUT_LOG_LOG_STREAM_HPP

#include <iosfwd>
#include "log/log_output.hpp"

namespace put { namespace log {

class LogStream {
public:
    LogStream(LogOutput*);
    LogStream(LogStream&&);
    ~LogStream();

    int write_bytes(const char*, size_t);

    LogStream& operator << (const char*);
    LogStream& operator << (const std::string&);
    LogStream& operator << (const int&);
    LogStream& operator << (const unsigned long long&);

private:
    LogOutput* output_;

    template <typename T>
    void writeInteger(T v);
};

}}



#endif //PUT_LOGSTREAM_HPP
