#ifndef WINDOWSIMPLEMENTATION_LOGGERFACTORYWINDOWS_H
#define WINDOWSIMPLEMENTATION_LOGGERFACTORYWINDOWS_H

#include <coreEngine/util/ILoggerFactory.h>
#include <windowsImplementation/LoggerWindows.h>

namespace cl{
    class LoggerFactoryWindows : public ILoggerFactory{
    public:
        std::unique_ptr<ILogger> createLogger(std::string tag){
            return std::unique_ptr<ILogger>(new LoggerWindows(tag));
        }
    };
}

#endif //WINDOWSIMPLEMENTATION_LOGGERFACTORYWINDOWS_H