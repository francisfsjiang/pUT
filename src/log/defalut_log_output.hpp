#ifndef PUT_DEFALUT_OUTPUT_HPP
#define PUT_DEFALUT_OUTPUT_HPP

#include "log/log_output.hpp"

namespace put { namespace log {

class DefaultLogOutput: public LogOutput{
public:

    virtual int append(const char*, size_t);
    virtual int flush();
    virtual ~DefaultLogOutput();

private:
    static const size_t k_BUFFER_SIZE = 1024;
    char buffer_[k_BUFFER_SIZE + 1];

};


}}



#endif //PUT_DEFALUT_OUTPUT_HPP
