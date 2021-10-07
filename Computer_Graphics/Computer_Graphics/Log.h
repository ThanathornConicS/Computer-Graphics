#pragma once

#include "pch.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Log
{
	class Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}

/* Core Log Macro */
#define L_SYSTEM_TRACE(...)	::Log::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define L_SYSTEM_INFO(...)	::Log::Log::GetCoreLogger()->info(__VA_ARGS__)
#define L_SYSTEM_WARN(...)	::Log::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define L_SYSTEM_ERROR(...)	::Log::Log::GetCoreLogger()->error(__VA_ARGS__)
#define L_SYSTEM_FATAL(...)	::Log::Log::GetCoreLogger()->fatal(__VA_ARGS__)

/* Client Log Macro */
#define L_TRACE(...)	::Log::Log::GetClientLogger()->trace(__VA_ARGS__)
#define L_INFO(...)		::Log::Log::GetClientLogger()->info(__VA_ARGS__)
#define L_WARN(...)		::Log::Log::GetClientLogger()->warn(__VA_ARGS__)
#define L_ERROR(...)	::Log::Log::GetClientLogger()->error(__VA_ARGS__)
#define L_FATAL(...)	::Log::Log::GetClientLogger()->fatal(__VA_ARGS__)
