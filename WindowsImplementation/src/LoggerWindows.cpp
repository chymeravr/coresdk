#include <iostream>
#include <unordered_map>
#include <windowsImplementation/LoggerWindows.h>

namespace cl{
	LoggerWindows::LoggerWindows(std::string tag){
		this->tag = tag;
	}
	void LoggerWindows::log(LOG_LEVEL level, std::string logString){
		std::unordered_map<LOG_LEVEL, std::string> logLevelString;
		logLevelString[LOG_DEBUG] = "DEBUG";
		logLevelString[LOG_INFO] = "INFO";
		logLevelString[LOG_WARN] = "WARN";
		logLevelString[LOG_ERROR] = "ERROR";
		logLevelString[LOG_FATAL] = "FATAL";
		std::cout << tag << " " << logLevelString[level] << " " << logString << std::endl;
	}
}