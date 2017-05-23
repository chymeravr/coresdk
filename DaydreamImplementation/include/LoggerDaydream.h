//
// Created by robin_chimera on 1/2/2017.
// contains logger implementation for daydream in android
//

#ifndef DAYDREAMIMPLEMENTATION_LOGGER_H
#define DAYDREAMIMPLEMENTATION_LOGGER_H

#include <string>
#include <coreEngine/util/ILogger.h>

namespace cl
{
class LoggerDaydream : public ILogger
{
public:
  LoggerDaydream(std::string tag);
  void log(LOG_LEVEL level, std::string logString);

private:
  std::string tag;
};
}

#endif //DAYDREAMIMPLEMENTATION_LOGGER_H
