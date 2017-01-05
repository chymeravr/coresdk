//
// Created by robin_chimera on 1/2/2017.
//

#include <iostream>
#include <unordered_map>
#include <LoggerAndroid.h>
#include <android/log.h>

//#define  MyLOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
//#define  MyLOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
//#define  MyLOGV(...)  __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG,__VA_ARGS__)
//#define  MyLOGI(...)  __android_log_print(ANDROID_LOG_INFO   , LOG_TAG,__VA_ARGS__)
//#define  MyLOGW(...)  __android_log_print(ANDROID_LOG_WARN   , LOG_TAG,__VA_ARGS__)
//#define  MyLOGF(...)  __android_log_print(ANDROID_LOG_FATAL   , LOG_TAG,__VA_ARGS__)

namespace cl{
    LoggerAndroid::LoggerAndroid(std::string tag){
        this->tag = tag;
    }
    void LoggerAndroid::log(LOG_LEVEL level, std::string logString){

        switch (level) {
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