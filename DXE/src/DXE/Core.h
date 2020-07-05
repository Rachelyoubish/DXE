#pragma once

#ifdef DXE_PLATFORM_WINDOWS
	#ifdef  DXE_BUILD_DLL
		#define DXE_API __declspec(dllexport)
	#else
		#define DXE_API __declspec(dllimport)
	#endif
#else
	#error DXE only supports Windows!
#endif

#ifdef DXE_ENABLE_ASSERTS
	#define DXE_ASSERT(x, ...) { if(!(x)) { DXE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define DXE_CORE_ASSERT(x, ...) { if(!(x)) { DXE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define DXE_ASSERT(x, ...)
	#define DXE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define DXE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)