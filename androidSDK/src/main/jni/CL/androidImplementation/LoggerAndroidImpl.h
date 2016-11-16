//
// Created by chimeralabs on 8/22/2016.
//

#ifndef ANDROIDSDK_LOGGERANDROIDIMPL_H
#define ANDROIDSDK_LOGGERANDROIDIMPL_H

#include <android/log.h>
#include <string>

#include <coreEngine/util/Logger.h>

namespace cl {

    class LoggerAndroidImpl : public Logger {
    public:
        LoggerAndroidImpl(std::string tag) : Logger(tag) {

        }

        void log(LOG_LEVEL level, std::string logString) {
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
    };
}
#endif //ANDROIDSDK_LOGGERANDROIDIMPL_H
