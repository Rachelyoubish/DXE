#include "dxepch.h"
#include "Application.h"

#include "DXE/Events/ApplicationEvent.h"
#include "DXE/Log.h"

namespace DXE {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e( 1280, 720 );
		if (e.IsInCategory( EventCategoryApplication ))
		{
			DXE_TRACE( e );
		}
		if (e.IsInCategory( EventCategoryInput ))
		{
			DXE_TRACE( e );
		}

		while (true);
	}
}