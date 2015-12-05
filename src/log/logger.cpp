#include "log/logger.hpp"

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>
#include <thread>

#include <iostream>

#include "log/defalut_log_output.hpp"

namespace put{ namespace log {

int gettid()
{
    std::thread::id this_id = std::this_thread::get_id();
    int t = static_cast<pid_t>(::syscall(SYS_thread_selfid));
    std::cout << this_id << t << std::endl;
    return t;
}


pid_t g_CACHED_TID = gettid();

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
             g_CACHED_TID,
             time_cached_tm_.tm_year + 1900,
             time_cached_tm_.tm_mon + 1,
             time_cached_tm_.tm_mday,
             time_cached_tm_.tm_hour,
             time_cached_tm_.tm_min,
             time_cached_tm_.tm_sec,
             time_timeval_now_.tv_usec,
             g_LogLevelName[log_level]

    );

    time_timeval_last_ = time_timeval_now_;
}

}}