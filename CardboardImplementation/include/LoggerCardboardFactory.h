//
// Created by robin_chimera on 1/2/2017.
//

#ifndef CARDBOARDIMPLEMENTATION_LOGGERFACTORY_H
#define CARDBOARDIMPLEMENTATION_LOGGERFACTORY_H

#include <coreEngine/util/ILoggerFactory.h>
#include <LoggerCardboard.h>

namespace cl
{
class LoggerCardboardFactory : public ILoggerFactory
{
  public:
    std::unique_ptr<ILogger> createLogger(std::string tag)
    {
        return std::unique_ptr<ILogger>(new LoggerCardboard(tag));
    }
};
}

#endif //CARDBOARDIMPLEMENTATION_LOGGERFACTORY_H
