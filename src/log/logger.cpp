#include "log/logger.hpp"

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#include "log/defalut_log_output.hpp"

namespace put{ namespace log {

__thread int g_CACHED_TID = 0;

pid_t gettid()
{
    if(!g_CACHED_TID){

#if defined(__linux__)
        g_CACHED_TID = static_cast<pid_t>(::syscall(SYS_gettid));
#elif defined(__unix__) || defined(__MACH__)
        g_CACHED_TID = static_cast<pid_t>(::syscall(SYS_thread_selfid));
#endif
    }
    return g_CACHED_TID;
}


Logger g_LOGGER = Logger(Logger::TRACE, new DefaultLogOutput());

char const * const g_LogLevelName[] = {
        "TRACE",
        "DEBUG",
        "INFO ",
        "WARN ",
        "ERROR",
        "FATAL"
};

void Logger::setLogLevel(LogLevel log_level) {
    log_level_ = log_level;
}

void Logger::setLogOutput(LogOutput* output) {
    delete output_;
    output_ = output;
}

Logger::Logger(LogLevel log_level, LogOutput * output) :
        log_level_(log_level),
        output_(output){
}

void Logger::update_header(LogLevel log_level) {
    gettimeofday(&time_timeval_now_, NULL);
    if(time_timeval_now_.tv_sec != time_timeval_last_.tv_sec) {
        gmtime_r(&(time_timeval_now_.tv_sec), &time_cached_tm_);
    }

    snprintf(header_buf_,
             sizeof(header_buf_),
             "%06d %4d%02d%02d %02d:%02d:%02d.%06d [%-5s] ",
             gettid(),
             time_cached_tm_.tm_year + 1900,
             time_cached_tm_.tm_mon + 1,
             time_cached_tm_.tm_mday,
             time_cached_tm_.tm_hour,
             time_cached_tm_.tm_min,
             time_cached_tm_.tm_sec,
             static_cast<int>(time_timeval_now_.tv_usec),
             g_LogLevelName[log_level]

    );

    time_timeval_last_ = time_timeval_now_;
}

}}