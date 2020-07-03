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