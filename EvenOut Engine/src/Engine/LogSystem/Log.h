#pragma once
#include "epch.h"
#define LOG_CORE(__VALUE__, __TYPE__) Engine::Log::PrintCore(__VALUE__, __TYPE__);
#define LOG_APP(__VALUE__, __TYPE__) Engine::Log::Print(__VALUE__, __TYPE__);

namespace Engine {

		enum LogLevel {
			LOG_INFO = 0,
			LOG_WARNING,
			LOG_ERROR
		};

	class Log {
	public:

		static std::string LevelToColor(LogLevel level);
		static std::string LevelToString(LogLevel level);

		template<typename T>
		static void Print(const T& value, LogLevel level)
		{
			std::cout << LevelToColor(level) << LevelToString(level)  << value << "\033[0m" << std::endl;
		}

		template<typename T>
		static void PrintCore(const T& value, LogLevel level)
		{
			std::cout << LevelToColor(level) << "[EveNOut Engine] - " << LevelToString(level) << value << "\033[0m" << std::endl;
		}
		
		//ad

	};
}