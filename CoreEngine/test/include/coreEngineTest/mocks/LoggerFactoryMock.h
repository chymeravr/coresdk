#ifndef COREENGINETEST_LOGGERFACTORYMOCK_H
#define COREENGINETEST_LOGGERFACTORYMOCK_H

#include <coreEngine/util/ILoggerFactory.h>
#include <coreEngineTest/mocks/LoggerMock.h>

namespace cl{
	class LoggerFactoryMock : public ILoggerFactory{
	public:
		std::unique_ptr<ILogger> createLogger(std::string tag){
			return std::unique_ptr<ILogger>(new ::testing::NiceMock<LoggerMock>());
		}
	};
}

#endif //COREENGINETEST_LOGGERFACTORYMOCK_H