//
// Created by robin_chimera on 1/2/2017.
//

#ifndef CARDBOARDIMPLEMENTATION_LOGGER_H
#define CARDBOARDIMPLEMENTATION_LOGGER_H

#include <string>
#include <coreEngine/util/ILogger.h>

namespace cl
{
class LoggerCardboard : public ILogger
{
public:
  LoggerCardboard(std::string tag);
  void log(LOG_LEVEL level, std::string logString);

private:
  std::string tag;
};
}

#endif //CARDBOARDIMPLEMENTATION_LOGGER_H
