#ifndef ANDROID_SDK_LOGGERFACTORYMOCK_H
#define ANDROID_SDK_LOGGERFACTORYMOCK_H

#include <coreEngine/util/ILoggerFactory.h>
#include <coreEngineTest/mocks/LoggerMock.h>
namespace cl{
	class LoggerFactoryMock : public ILoggerFactory{
		std::unique_ptr<ILogger> createLogger(std::string tag){
			return std::unique_ptr<ILogger>(new LoggerMock());
		}
	};
}

#endif //ANDROID_SDK_LOGGERFACTORYMOCK_H