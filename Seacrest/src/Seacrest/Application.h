#pragma once

#include "Core.h"

#include "Window.h"

#include "Seacrest/LayerStack.h"
#include "Events/Event.h"
#include "Seacrest/Events/ApplicationEvent.h"

#include "Seacrest/Core/Timestep.h"

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

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize( WindowResizeEvent& e );
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		LARGE_INTEGER StartingTime, CurrentTime, Frequency;

		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT. 
	Application* CreateApplication();
}