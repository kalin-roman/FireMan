#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

enum LogType{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntrY{
    LogType type;
    std::string messsage;
};

class Logger {
    public:
        static std::vector<LogEntrY> message;
        static void Log(const std::string& message);
        static void Err(const std::string& message);
    private:
        static void Print(const std::string& message, std::string color, LogType type);
};
#endif