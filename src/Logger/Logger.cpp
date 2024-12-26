#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */

std::vector<LogEntrY> Logger::message;


void Logger::Print(const std::string& message, std::string color, LogType type){
    auto t = std::time(nullptr);
    auto tm = std::localtime(&t);
    std::string output(30,'\0');
    strftime(&output[0], output.size(),"%d/%b/%Y %H:%M:%S - ",tm);
    LogEntrY logEntry;
    logEntry.type = type;
    logEntry.messsage = output + message;
    std::cout<<logEntry.messsage << std::endl << std::endl;
};

void Logger::Log(const std::string& message){
    Logger::Print(message, GREEN, LOG_INFO);
};

void Logger::Err(const std::string& message){
    Logger::Print(message, RED, LOG_ERROR);
};
