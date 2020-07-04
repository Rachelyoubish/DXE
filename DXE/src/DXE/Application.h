#pragma once

#include "Core.h"

#include "Window.h"

#include "DXE/LayerStack.h"
#include "Events/Event.h"
#include "DXE/Events/ApplicationEvent.h"

namespace DXE {

	class DXE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent( Event& e );

		void PushLayer( Layer* layer );
		void PushOverlay( Layer* layer );
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in CLIENT. 
	Application* CreateApplication();
}