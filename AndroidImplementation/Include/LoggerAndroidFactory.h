//
// Created by robin_chimera on 1/2/2017.
//

#ifndef GRAPHICSSDK_LOGGERANDROIDFACTORY_H
#define GRAPHICSSDK_LOGGERANDROIDFACTORY_H

#include <coreEngine/util/ILoggerFactory.h>
#include <LoggerAndroid.h>

namespace cl{
    class LoggerFactoryAndroid : public ILoggerFactory{
    public:
        std::unique_ptr<ILogger> createLogger(std::string tag){
            return std::unique_ptr<ILogger>(new LoggerAndroid(tag));
        }
    };
}

#endif //GRAPHICSSDK_LOGGERANDROIDFACTORY_H
