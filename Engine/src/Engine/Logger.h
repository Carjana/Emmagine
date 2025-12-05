#pragma once

#include <memory>

#include "Core.h"
#include "SDL3/SDL_log.h"
#include "spdlog/spdlog.h"

inline const char* SDL_LOG_CATEGORIES[] =
{
	"SDL_APPLICATION",
	"SDL_ERROR",
	"SDL_ASSERT",
	"SDL_SYSTEM",
	"SDL_AUDIO",
	"SDL_VIDEO",
	"SDL_RENDER",
	"SDL_INPUT",
	"SDL_TEST",
	"SDL_GPU",
	"SDL_RESERVED2",
	"SDL_RESERVED3",
	"SDL_RESERVED4",
	"SDL_RESERVED5",
	"SDL_RESERVED6",
	"SDL_RESERVED7",
	"SDL_RESERVED8",
	"SDL_RESERVED9",
	"SDL_RESERVED10",
	"SDL_CUSTOM"
};

namespace Emma
{
	class EMMA_API Logger
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger(){return coreLogger;}
		static std::shared_ptr<spdlog::logger>& GetClientLogger(){return clientLogger;}

		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;

	private:
		static void SDL_Log(void * userdata, int category, SDL_LogPriority priority, const char * message);
	};
}

#ifdef DEBUG
#define LOG_CORE_TRACE(...) Emma::Logger::GetCoreLogger()->trace(__VA_ARGS__);
#define LOG_CORE_INFO(...) Emma::Logger::GetCoreLogger()->info(__VA_ARGS__);
#define LOG_CORE_WARN(...) Emma::Logger::GetCoreLogger()->warn(__VA_ARGS__);
#define LOG_CORE_ERROR(...) Emma::Logger::GetCoreLogger()->error(__VA_ARGS__);
#define LOG_CORE_CRITICAL(...) Emma::Logger::GetCoreLogger()->critical(__VA_ARGS__);

#define LOG_APP_TRACE(...) Emma::Logger::GetClientLogger()->trace(__VA_ARGS__);
#define LOG_APP_INFO(...) Emma::Logger::GetClientLogger()->info(__VA_ARGS__);
#define LOG_APP_WARN(...) Emma::Logger::GetClientLogger()->warn(__VA_ARGS__);
#define LOG_APP_ERROR(...) Emma::Logger::GetClientLogger()->error(__VA_ARGS__);
#define LOG_APP_CRITICAL(...) Emma::Logger::GetClientLogger()->critical(__VA_ARGS__);

#ifdef EMMA_BUILD_DLL

#define LOG_TRACE(...) LOG_CORE_TRACE(__VA_ARGS__)
#define LOG_INFO(...)  LOG_CORE_INFO(__VA_ARGS__)
#define LOG_WARN(...)  LOG_CORE_WARN(__VA_ARGS__)
#define LOG_ERROR(...)  LOG_CORE_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...)  LOG_CORE_CRITICAL(__VA_ARGS__)

#else

#define LOG_TRACE(...) LOG_APP_TRACE(__VA_ARGS__)
#define LOG_INFO(...)  LOG_APP_INFO(__VA_ARGS__)
#define LOG_WARN(...)  LOG_APP_WARN(__VA_ARGS__)
#define LOG_ERROR(...)  LOG_APP_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...)  LOG_APP_CRITICAL(__VA_ARGS__)
#endif

#else

#define LOG_CORE_TRACE(...)
#define LOG_CORE_INFO(...)
#define LOG_CORE_WARN(...)
#define LOG_CORE_ERROR(...)
#define LOG_CORE_CRITICAL(...)

#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)

#endif
