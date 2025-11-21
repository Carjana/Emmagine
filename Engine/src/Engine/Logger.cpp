#include "Logger.h"

#include "SDL3/SDL.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Emma
{
	std::shared_ptr<spdlog::logger> Logger::coreLogger;
	std::shared_ptr<spdlog::logger> Logger::clientLogger;

	void Logger::Init()
	{
		// SDL_SetLogOutputFunction();

		// https://github.com/gabime/spdlog/wiki/Custom-formatting
		spdlog::set_pattern("%^[%T][%l] %n: %v%$");

		coreLogger = spdlog::stdout_color_mt("EMMA");
		coreLogger->set_level(spdlog::level::trace);
		clientLogger = spdlog::stdout_color_mt("APP");
		clientLogger->set_level(spdlog::level::trace);
	}

	void Logger::Info(const char *message)
	{

	}
}
