#include "epch.h"
#include "Log.h"

namespace Engine {
	std::string Log::LevelToColor(LogLevel level) {
		switch (level)
		{
		case LogLevel::LOG_INFO:
			return "\033[1m\033[36m";
			break;
		case LogLevel::LOG_WARNING:
			return  "\033[1m\033[33m";
			break;
		case LogLevel::LOG_ERROR:
			return "\033[1m\033[31m";
			break;
		}

		return "";
	};

	std::string Log::LevelToString(LogLevel level) {
		switch (level)
		{
		case LogLevel::LOG_INFO:
			return "[INFO] ";
			break;
		case LogLevel::LOG_WARNING:
			return "[WARNING] ";
			break;
		case LogLevel::LOG_ERROR:
			return "[ERROR] ";
			break;
		}

		return "";
	};

}