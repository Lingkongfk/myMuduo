
#include "logger.h"
#include "Timestamp.h"


//实例化单例对象
Logger& Logger::instance(){
    static Logger logger;
    return logger;
}
//设置日志级别
void Logger::setLogLevel(LogLevel level){
    logLevel_ = level;
}
//写日志
void Logger::log(std::string msg){
    switch(logLevel_){
    case LogLevel::INFO:
        std::cout << "[INFO]";
        break;
    case LogLevel::ERROR:
        std::cout << "[ERROR]";
        break;
    case LogLevel::FATAL:
        std::cout << "[FATAL]";
        break;
    case LogLevel::DEBUG:
        std::cout << "[DEBUG]";
        break;
    }

    //打印时间和msg
    std::cout <<Timestamp::now().toString() << " : "<< msg << std::endl;
}
