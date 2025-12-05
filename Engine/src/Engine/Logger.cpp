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
		// https://github.com/gabime/spdlog/wiki/Custom-formatting
		spdlog::set_pattern("%^[%T][%l] %n: %v%$");

		coreLogger = spdlog::stdout_color_mt("EMMA");
		coreLogger->set_level(spdlog::level::trace);
		clientLogger = spdlog::stdout_color_mt("APP");
		clientLogger->set_level(spdlog::level::trace);

		SDL_SetLogPriorities(SDL_LOG_PRIORITY_TRACE);
		SDL_SetLogOutputFunction(SDL_Log, nullptr);
	}

	void Logger::SDL_Log(void * userdata, int category, SDL_LogPriority priority, const char * message)
	{
		switch (priority)
		{
			case SDL_LOG_PRIORITY_TRACE:
			case SDL_LOG_PRIORITY_VERBOSE:
			case SDL_LOG_PRIORITY_DEBUG:
				LOG_CORE_TRACE("({}) {}", SDL_LOG_CATEGORIES[category], message);
				break;
			case SDL_LOG_PRIORITY_INFO:
				LOG_CORE_INFO("({}) {}", SDL_LOG_CATEGORIES[category], message);
				break;
			case SDL_LOG_PRIORITY_WARN:
				LOG_CORE_WARN("({}) {}", SDL_LOG_CATEGORIES[category], message);
				break;
			case SDL_LOG_PRIORITY_ERROR:
				LOG_CORE_ERROR("({}) {}", SDL_LOG_CATEGORIES[category], message);
				break;
			case SDL_LOG_PRIORITY_CRITICAL:
				LOG_CORE_CRITICAL("({}) {}", SDL_LOG_CATEGORIES[category], message);
				break;
			default:
				LOG_CORE_TRACE("({}) {}", SDL_LOG_CATEGORIES[category], message);
				break;
		}
	}
}
