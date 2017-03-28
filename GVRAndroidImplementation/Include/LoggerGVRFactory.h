//
// Created by robin_chimera on 1/2/2017.
//

#ifndef ANDROIDIMPLEMENTATION_LOGGERGVRFACTORY_H
#define ANDROIDIMPLEMENTATION_LOGGERGVRFACTORY_H

#include <coreEngine/util/ILoggerFactory.h>
#include <LoggerGVR.h>

namespace cl
{
class LoggerFactoryGVR : public ILoggerFactory
{
  public:
    std::unique_ptr<ILogger> createLogger(std::string tag)
    {
        return std::unique_ptr<ILogger>(new LoggerGVR(tag));
    }
};
}

#endif //ANDROIDIMPLEMENTATION_LOGGERGVRFACTORY_H
