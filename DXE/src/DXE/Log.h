#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace DXE {

	class DXE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger;  }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define DXE_CORE_TRACE(...)    ::DXE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DXE_CORE_INFO(...)     ::DXE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DXE_CORE_WARN(...)     ::DXE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DXE_CORE_ERROR(...)    ::DXE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DXE_CORE_FATAL(...)    ::DXE::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define DXE_TRACE(...)         ::DXE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DXE_INFO(...)          ::DXE::Log::GetClientLogger()->info(__VA_ARGS__)
#define DXE_WARN(...)          ::DXE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DXE_ERROR(...)         ::DXE::Log::GetClientLogger()->error(__VA_ARGS__)
#define DXE_FATAL(...)         ::DXE::Log::GetClientLogger()->critical(__VA_ARGS__)