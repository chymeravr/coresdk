#ifndef ANDROIDSDK_LOGGERMOCK_H
#define ANDROIDSDK_LOGGERMOCK_H

#include <gmock/gmock.h>
#include <coreEngine/util/ILogger.h>

namespace cl{
	class LoggerMock : public ILogger{
	public:
		MOCK_METHOD2(log, void(LOG_LEVEL level, std::string logString));
	};
}

#endif //ANDROIDSDK_LOGGERMOCK_H