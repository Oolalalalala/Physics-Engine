#pragma once
#pragma once

#include "Base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


namespace Olala {

	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}

#define CORE_LOG_TRACE(...)    ::Olala::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_LOG_INFO(...)     ::Olala::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...)     ::Olala::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...)    ::Olala::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_LOG_CRITICAL(...) ::Olala::Log::GetCoreLogger()->critical(__VA_ARGS__)
							   
#define LOG_TRACE(...)         ::Olala::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          ::Olala::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::Olala::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::Olala::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::Olala::Log::GetClientLogger()->critical(__VA_ARGS__)