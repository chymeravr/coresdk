//
// Created by robin_chimera on 1/2/2017.
//

#ifndef GRAPHICSSDK_LOGGERANDROID_H
#define GRAPHICSSDK_LOGGERANDROID_H


#include <string>
#include <coreEngine/util/ILogger.h>

namespace cl{
    class LoggerAndroid : public ILogger{
    public:
        LoggerAndroid(std::string tag);
        void log(LOG_LEVEL level, std::string logString);
    private:
        std::string tag;
    };
}

#endif //GRAPHICSSDK_LOGGERANDROID_H
