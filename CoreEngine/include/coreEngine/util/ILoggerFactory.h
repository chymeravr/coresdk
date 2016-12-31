//
// Created by chimeralabs on 10/6/2016.
//

#ifndef ANDROIDSDK_LOGGERFACTORY_H
#define ANDROIDSDK_LOGGERFACTORY_H

#include <memory>
#include <coreEngine/util/ILogger.h>

namespace cl{
	class ILoggerFactory{
	public:
		virtual std::unique_ptr<ILogger> createLogger(std::string tag) = 0;
	};
}

#endif //ANDROIDSDK_LOGGERFACTORY_H
