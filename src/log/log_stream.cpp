#include "log/log_stream.hpp"

#include <string>
#include <cstring>

namespace put { namespace log {

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

const char digitsHex[] = "0123456789ABCDEF";

const int k_MAX_NUMERIC_SIZE = 32;

// Efficient Integer to String Conversions, by Matthew Wilson.
template<typename T>
size_t convert(char buf[], T value)
{
    T i = value;
    char* p = buf;

    do
    {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0)
    {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
    uintptr_t i = value;
    char* p = buf;

    do
    {
        int lsd = static_cast<int>(i % 16);
        i /= 16;
        *p++ = digitsHex[lsd];
    } while (i != 0);

    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

template <typename T>
void LogStream::writeInteger(T v) {
    if (output_->avail() > k_MAX_NUMERIC_SIZE) {
        size_t size = convert(output_->buf(), v);
        output_->add(size);
    }
}


LogStream::LogStream(LogOutput* output)
        : output_(output){
}

LogStream::~LogStream() {
    output_ -> append("\n", 1);
    output_->flush();
}

int LogStream::write_bytes(const char *buf, size_t len) {
    return output_ -> append(buf, len);
}

LogStream& LogStream::operator<<(const char * str) {
    size_t len = strlen(str);
    output_ -> append(str, len);
    return *this;
}


LogStream& LogStream::operator << (const std::string& s) {
    output_ -> append(s.data(), s.size());
    return *this;
}

LogStream& LogStream::operator<<(const int& v) {
    writeInteger(v);
    return *this;
}

LogStream& LogStream::operator <<(const unsigned long long& v) {
    writeInteger(v);
    return *this;
}

}}
