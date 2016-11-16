//
// Created by chimeralabs on 10/6/2016.
//

#ifndef ANDROIDSDK_LOGGERFACTORY_H
#define ANDROIDSDK_LOGGERFACTORY_H

#include <memory>
#include <coreEngine/util/Logger.h>

namespace cl{
    class LoggerFactory{
    private:
        static std::unique_ptr<LoggerFactory> loggerFactoryInstancePtr;

    public:
        static LoggerFactory &getInstance(){
            return *loggerFactoryInstancePtr;
        }
        static void setInstance(std::unique_ptr<LoggerFactory> instancePtr){
            loggerFactoryInstancePtr = std::move(instancePtr);
        }
        virtual Logger &getLoggerInstance(std::string tag) = 0;
    };
}

#endif //ANDROIDSDK_LOGGERFACTORY_H
