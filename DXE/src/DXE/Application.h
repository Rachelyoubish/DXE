#pragma once

#include "Core.h"

#include "Window.h"

#include "DXE/LayerStack.h"
#include "Events/Event.h"
#include "DXE/Events/ApplicationEvent.h"

#include "DXE/ImGui/ImGuiLayer.h"

#include "Platform/DirectX/DirectXContext.h"

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

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize( WindowResizeEvent& e );

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		D3DContext* m_Context;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT. 
	Application* CreateApplication();
}