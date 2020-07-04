#include "dxepch.h"
#include "Application.h"

#include "DXE/Events/ApplicationEvent.h"
#include "DXE/Log.h"

namespace DXE {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>( Window::Create() );
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}