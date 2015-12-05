#ifndef PUT_LOG_LOGGER_HPP
#define PUT_LOG_LOGGER_HPP

#include <string.h>
#include <ctime>
#include <type_traits>

#include "log/log_stream.hpp"
#include "log/defalut_log_output.hpp"

namespace put { namespace log{


class Logger {
public:
    enum LogLevel{
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
    };

    Logger(LogLevel, LogOutput*);

    void setLogLevel(LogLevel log_level);

    void setLogOutput(LogOutput*);

    inline LogStream stream(const char* filename, int line, const LogLevel& log_level) {
        LogStream stream = LogStream(output_);
        update_header(log_level);
        stream.write_bytes(header_buf_, sizeof(header_buf_));
        stream << filename << ":" << line;

        return stream;
    }

    void update_header(LogLevel);

private:

    LogLevel log_level_;

    LogOutput* output_;

    struct timeval time_timeval_now_;
    struct timeval time_timeval_last_;
    struct tm time_cached_tm_;
    char header_buf_[41];



};


extern Logger g_LOGGER;

#define LOG_TRACE put::log::g_LOGGER.stream(__FILENAME__, __LINE__, put::log::Logger::TRACE)
#define LOG_DEBUG put::log::g_LOGGER.stream(__FILENAME__, __LINE__, put::log::Logger::DEBUG)
#define LOG_INFO  put::log::g_LOGGER.stream(__FILENAME__, __LINE__, put::log::Logger::INFO )
#define LOG_WARN  put::log::g_LOGGER.stream(__FILENAME__, __LINE__, put::log::Logger::WARN )
#define LOG_ERROR put::log::g_LOGGER.stream(__FILENAME__, __LINE__, put::log::Logger::ERROR)
#define LOG_FATAL put::log::g_LOGGER.stream(__FILENAME__, __LINE__, put::log::Logger::FATAL)



}}


#endif //PUT_LOGGER_HPP
