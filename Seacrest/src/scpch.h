#pragma once

// STL includes
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

// Project logger include 
#include "Seacrest/Log.h"

#ifdef SEACREST_PLATFORM_WINDOWS
	// System includes
	#include <Windows.h>
	#include <windowsx.h>
	#include <wrl.h>

    // Direct3D includes
	#include <d3d11.h>
    #include <d3dcompiler.h>

	// Link library dependencies  
	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "dxgi.lib")
#endif