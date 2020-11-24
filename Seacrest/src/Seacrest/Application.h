#pragma once

#include "Core.h"

#include "Window.h"

#include "Seacrest/LayerStack.h"
#include "Events/Event.h"
#include "Seacrest/Events/ApplicationEvent.h"

#include "Seacrest/ImGui/ImGuiLayer.h"

#include "Seacrest/Renderer/Shader.h"

#include "Seacrest/Renderer/Buffer.h"

namespace Seacrest {

	class SEACREST_API Application
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
		bool m_Running = true;
		LayerStack m_LayerStack;

		GraphicsContext* m_Context;
		// TEMP
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		UINT m_Vertex;
		UINT m_Index;
		//UINT m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT. 
	Application* CreateApplication();
}