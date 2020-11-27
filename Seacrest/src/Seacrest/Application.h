#pragma once

#include "Core.h"

#include "Window.h"

#include "Seacrest/LayerStack.h"
#include "Events/Event.h"
#include "Seacrest/Events/ApplicationEvent.h"

#include "Seacrest/ImGui/ImGuiLayer.h"

#include "Seacrest/Renderer/Shader.h"

#include "Seacrest/Renderer/Buffer.h"

#include "Seacrest/Renderer/InputLayout.h"

namespace Seacrest {

	class SEACREST_API Application
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

		GraphicsContext* m_Context;
		std::shared_ptr<InputLayout> m_InputLayout;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<InputLayout> m_SquareInput;
		std::shared_ptr<Shader> m_SquareShader;
	private: // TEMP
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT. 
	Application* CreateApplication();
}