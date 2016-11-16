//
// Created by chimeralabs on 10/6/2016.
//

#ifndef ANDROIDSDK_LOGGERFACTORYANDROIDIMPL_H
#define ANDROIDSDK_LOGGERFACTORYANDROIDIMPL_H

#include <memory>
#include <string>
#include <unordered_map>

#include <coreEngine/util/Logger.h>
#include <coreEngine/util/LoggerFactory.h>
#include <androidImplementation/LoggerAndroidImpl.h>

namespace cl {
    class LoggerFactoryAndroidImpl : public LoggerFactory {
    private:
        std::unordered_map<std::string, std::unique_ptr<Logger> > loggerPtrs;
    public:
        Logger &getLoggerInstance(std::string tag) {
            tag = "CL" + tag;
            if (loggerPtrs.find(tag) == loggerPtrs.cend()) {
                loggerPtrs[tag] = std::unique_ptr<LoggerAndroidImpl>(new LoggerAndroidImpl(tag));
            }
            return *loggerPtrs[tag];
        }
    };
}
#endif //ANDROIDSDK_LOGGERFACTORYANDROIDIMPL_H
