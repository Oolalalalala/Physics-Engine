#include "pch.h"
#include "Log.h"

#pragma warning(push, 0)
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)

namespace Olala {

	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::sink_ptr logSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

		logSink->set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = std::make_shared<spdlog::logger>("Olala", logSink);
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("Client", logSink);
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}

}