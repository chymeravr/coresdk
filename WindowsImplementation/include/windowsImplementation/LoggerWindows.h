#ifndef WINDOWSIMPLEMENTATION_LOGGERWINDOWS_H
#define WINDOWSIMPLEMENTATION_LOGGERWINDOWS_H

#include <string>
#include <coreEngine/util/ILogger.h>

namespace cl{
    class LoggerWindows : public ILogger{
    public:
        LoggerWindows(std::string tag);
        void log(LOG_LEVEL level, std::string logString);
    private:
        std::string tag;
    };
}

#endif //WINDOWSIMPLEMENTATION_LOGGERWINDOWS_H