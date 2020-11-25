#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Seacrest/Log.h"

#ifdef SEACREST_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <windowsx.h>
	#include <d3d11.h>

	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "dxgi.lib")
#endif