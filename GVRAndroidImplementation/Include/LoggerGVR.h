//
// Created by robin_chimera on 1/2/2017.
//

#ifndef ANDROIDIMPLEMENTATION_LOGGERGVR_H
#define ANDROIDIMPLEMENTATION_LOGGERGVR_H

#include <string>
#include <coreEngine/util/ILogger.h>

namespace cl
{
class LoggerGVR : public ILogger
{
public:
  LoggerGVR(std::string tag);
  void log(LOG_LEVEL level, std::string logString);

private:
  std::string tag;
};
}

#endif //ANDROIDIMPLEMENTATION_LOGGERGVR_H
