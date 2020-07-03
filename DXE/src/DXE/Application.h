#pragma once

#include "Core.h"

namespace DXE {

	class DXE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT. 
	Application* CreateApplication();
}