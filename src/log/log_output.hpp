#ifndef PUT_LOG_LOG_OUTPUT_HPP
#define PUT_LOG_LOG_OUTPUT_HPP

#include <cstdio>

namespace put { namespace log {

class LogOutput {
public:
    virtual int append(const char*, size_t) = 0;
    virtual int add(size_t) = 0;
    virtual int flush() = 0;
    virtual char* buf() = 0;
    virtual size_t avail() = 0;
    virtual ~LogOutput(){};
};


}}

#endif //PUT_LOG_OUTPUT_HPP
