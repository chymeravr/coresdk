//
// Created by robin_chimera on 1/2/2017.
//

#ifndef ANDROIDIMPLEMENTATION_LOGGEROVRM_H
#define ANDROIDIMPLEMENTATION_LOGGEROVRM_H

#include <string>
#include <coreEngine/util/ILogger.h>

namespace cl
{
class LoggerOVRM : public ILogger
{
  public:
    LoggerOVRM(std::string tag);
    void log(LOG_LEVEL level, std::string logString);

  private:
    std::string tag;
};
}

#endif //ANDROIDIMPLEMENTATION_LOGGEROVRM_H
