#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/logger.h"

namespace Emma
{
	class EMMA_API Logger
	{
	public:
		static void Init();
		static void Info(const char* message);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger(){return coreLogger;}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger(){return clientLogger;}

		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
}

#define LOG_CORE(...) Emma::Logger::GetCoreLogger()->trace(__VA_ARGS__);
#define LOG_CORE_INFO(...) Emma::Logger::GetCoreLogger()->info(__VA_ARGS__);
#define LOG_CORE_WARN(...) Emma::Logger::GetCoreLogger()->warn(__VA_ARGS__);
#define LOG_CORE_ERROR(...) Emma::Logger::GetCoreLogger()->error(__VA_ARGS__);
#define LOG_CORE_CRITICAL(...) Emma::Logger::GetCoreLogger()->critical(__VA_ARGS__);

#define LOG(...) Emma::Logger::GetClientLogger()->trace(__VA_ARGS__);
#define LOG_INFO(...) Emma::Logger::GetClientLogger()->info(__VA_ARGS__);
#define LOG_WARN(...) Emma::Logger::GetClientLogger()->warn(__VA_ARGS__);
#define LOG_ERROR(...) Emma::Logger::GetClientLogger()->error(__VA_ARGS__);
#define LOG_CRITICAL(...) Emma::Logger::GetClientLogger()->critical(__VA_ARGS__);