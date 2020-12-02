#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include "ImGui/ImGuiConsole.h"

namespace Seacrest {

	class SEACREST_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define SEACREST_CORE_TRACE(...)    ::Seacrest::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SEACREST_CORE_INFO(...)     ::Seacrest::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SEACREST_CORE_WARN(...)     ::Seacrest::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SEACREST_CORE_ERROR(...)    ::Seacrest::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SEACREST_CORE_CRITICAL(...)    ::Seacrest::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SEACREST_TRACE(...)         ::Seacrest::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SEACREST_INFO(...)          ::Seacrest::Log::GetClientLogger()->info(__VA_ARGS__)
#define SEACREST_WARN(...)          ::Seacrest::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SEACREST_ERROR(...)         ::Seacrest::Log::GetClientLogger()->error(__VA_ARGS__)
#define SEACREST_CRITICAL(...)         ::Seacrest::Log::GetClientLogger()->critical(__VA_ARGS__)
