#ifndef PUT_DEFALUT_OUTPUT_HPP
#define PUT_DEFALUT_OUTPUT_HPP

#include "log/log_output.hpp"

namespace put { namespace log {

class DefaultLogOutput: public LogOutput{
public:

    virtual int append(const char*, size_t);
    virtual int add(size_t);
    virtual int flush();
    virtual char* buf();
    virtual size_t avail();
    virtual ~DefaultLogOutput();

private:
    static const size_t k_BUFFER_SIZE = 1024;
    char buffer_[k_BUFFER_SIZE + 1];
    char* cur_ = buffer_;
    size_t avail_ = k_BUFFER_SIZE;
};


}}



#endif //PUT_DEFALUT_OUTPUT_HPP
