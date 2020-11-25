#pragma once

#include <memory>

#ifdef SEACREST_PLATFORM_WINDOWS
#if SEACREST_DYNAMIC_LINK
	#ifdef  SEACREST_BUILD_DLL
		#define SEACREST_API __declspec(dllexport)
	#else
		#define SEACREST_API __declspec(dllimport)
	#endif
#else
	#define SEACREST_API
#endif
#else
	#error SEACREST only supports Windows!
#endif

#ifdef SEACREST_DEBUG
	#define SEACREST_ENABLE_ASSERTS
#endif

#ifdef SEACREST_ENABLE_ASSERTS
	#define SEACREST_ASSERT(x, ...) { if(!(x)) { SEACREST_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define SEACREST_CORE_ASSERT(x, ...) { if(!(x)) { SEACREST_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SEACREST_ASSERT(x, ...)
	#define SEACREST_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SEACREST_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Seacrest {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}