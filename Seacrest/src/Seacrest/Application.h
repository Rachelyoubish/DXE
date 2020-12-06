#pragma once

#include "Core.h"

#include "Window.h"

#include "Seacrest/LayerStack.h"
#include "Events/Event.h"
#include "Seacrest/Events/ApplicationEvent.h"

#include "Seacrest/ImGui/ImGuiLayer.h"

namespace Seacrest {

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

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
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT. 
	Application* CreateApplication();
}