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

#define OLA_CORE_TRACE(...)    ::Olala::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OLA_CORE_INFO(...)     ::Olala::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OLA_CORE_WARN(...)     ::Olala::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OLA_CORE_ERROR(...)    ::Olala::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OLA_CORE_CRITICAL(...) ::Olala::Log::GetCoreLogger()->critical(__VA_ARGS__)
							   
#define OLA_TRACE(...)         ::Olala::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OLA_INFO(...)          ::Olala::Log::GetClientLogger()->info(__VA_ARGS__)
#define OLA_WARN(...)          ::Olala::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OLA_ERROR(...)         ::Olala::Log::GetClientLogger()->error(__VA_ARGS__)
#define OLA_CRITICAL(...)      ::Olala::Log::GetClientLogger()->critical(__VA_ARGS__)