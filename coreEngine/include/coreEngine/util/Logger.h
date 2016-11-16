//
// Created by chimeralabs on 8/8/2016.
//

/**
 * Logger abstract class (interface). This class is used by classes to be used across platforms.
 * A platform specific implementation of this interface needs to be provided by using modules
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>


namespace cl {
    enum LOG_LEVEL {
        LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL
    };

    class Logger {
    protected:
        std::string tag;
    public:
        Logger(std::string tag) {
            this->tag = tag;
        }

        virtual void log(LOG_LEVEL level, std::string logString) = 0;
    };
}
#endif //LOGGER_H
