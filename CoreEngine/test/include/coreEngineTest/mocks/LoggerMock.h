#ifndef COREENGINETEST_LOGGERMOCK_H
#define COREENGINETEST_LOGGERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/util/ILogger.h>

namespace cl{
	class LoggerMock : public ILogger{
		MOCK_METHOD2(log, void(LOG_LEVEL level, std::string logString));
	};
}

#endif //COREENGINETEST_LOGGERMOCK_H