//
// Created by robin_chimera on 1/2/2017.
//

#ifndef ANDROIDIMPLEMENTATION_LOGGEROVRMFACTORY_H
#define ANDROIDIMPLEMENTATION_LOGGEROVRMFACTORY_H

#include <coreEngine/util/ILoggerFactory.h>
#include <LoggerOVRM.h>

namespace cl{
    class LoggerFactoryOVRM : public ILoggerFactory{
    public:
        std::unique_ptr<ILogger> createLogger(std::string tag){
            return std::unique_ptr<ILogger>(new LoggerOVRM(tag));
        }
    };
}

#endif //ANDROIDIMPLEMENTATION_LOGGEROVRMFACTORY_H
