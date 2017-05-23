//
// Created by robin_chimera on 1/2/2017.
//

#ifndef DAYDREAMIMPLEMENTATION_LOGGERFACTORY_H
#define DAYDREAMIMPLEMENTATION_LOGGERFACTORY_H

#include <coreEngine/util/ILoggerFactory.h>
#include <LoggerDaydream.h>

namespace cl
{
class LoggerDaydreamFactory : public ILoggerFactory
{
  public:
    std::unique_ptr<ILogger> createLogger(std::string tag)
    {
        return std::unique_ptr<ILogger>(new LoggerDaydream(tag));
    }
};
}

#endif //DAYDREAMIMPLEMENTATION_LOGGERFACTORY_H
