#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "DXE/Log.h"

#ifdef DXE_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <windowsx.h>
	#include <d3d11.h>

	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "dxgi.lib")
#endif