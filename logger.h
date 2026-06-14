#pragma once
#include <iostream>
#include <string>

#include "noncopyable.h"
//定义日志的级别 
//INFO正常的日志输出  
//ERROR 不影响程序  
//FATAL系统无法向下运行 
//DEBUG调试信息非常多的

// LOG_INFO("%s %d", arg1, arg2)
#define LOG_INFO(logmsgFormat, ...) \
    do {\
        Logger &logger = Logger::instance();\
        logger.setLogLevel(LogLevel::INFO);  \
        char buf[1024] = {0};\
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);\
        logger.log(buf);\
    }while(0)

#define LOG_ERROR(logmsgFormat, ...) \
    do {\
        Logger &logger = Logger::instance();\
        logger.setLogLevel(LogLevel::ERROR);  \
        char buf[1024] = {0};\
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);\
        logger.log(buf);\
    }while(0)

#define LOG_FATAL(logmsgFormat, ...) \
    do {\
        Logger &logger = Logger::instance();\
        logger.setLogLevel(LogLevel::FATAL);  \
        char buf[1024] = {0};\
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);\
        logger.log(buf);\
    }while(0)

#define LOG_DEBUG(logmsgFormat, ...) \
    do {\
        Logger &logger = Logger::instance();\
        logger.setLogLevel(LogLevel::DEBUG);  \
        char buf[1024] = {0};\
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);\
        logger.log(buf);\
    }while(0)



enum class LogLevel{
    INFO, //普通信息
    ERROR,//错误信息
    FATAL,//core信息
    DEBUG //调试信息
};


//日志我们写成单例模式
class Logger : noncopyable
{
public:
    //实例化单例对象
    static Logger& instance();
    //设置日志级别
    void setLogLevel(LogLevel level);
    //写日志
    void log(std::string msg);
private:
    Logger() {}
    LogLevel logLevel_;
};

