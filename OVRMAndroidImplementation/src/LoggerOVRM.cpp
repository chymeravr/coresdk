//
// Created by robin_chimera on 1/2/2017.
//

#include <iostream>
#include <unordered_map>
#include <LoggerOVRM.h>
#include <android/log.h>

namespace cl
{
LoggerOVRM::LoggerOVRM(std::string tag)
{
    this->tag = tag;
}
void LoggerOVRM::log(LOG_LEVEL level, std::string logString)
{

    switch (level)
    {
    case LOG_DEBUG:
        __android_log_print(ANDROID_LOG_DEBUG, this->tag.c_str(), logString.c_str());
        break;
    case LOG_INFO:
        __android_log_print(ANDROID_LOG_INFO, this->tag.c_str(), logString.c_str());
        break;
    case LOG_WARN:
        __android_log_print(ANDROID_LOG_WARN, this->tag.c_str(), logString.c_str());
        break;
    case LOG_ERROR:
        __android_log_print(ANDROID_LOG_ERROR, this->tag.c_str(), logString.c_str());
        break;
    case LOG_FATAL:
        __android_log_print(ANDROID_LOG_FATAL, this->tag.c_str(), logString.c_str());
        break;
    }
}
}