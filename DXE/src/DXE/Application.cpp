#include "dxepch.h"
#include "Application.h"

#include "DXE/Log.h"

#include "Input.h"

namespace DXE {

#define BIND_EVENT_FN(x) std::bind( &Application::x, this, std::placeholders::_1 )

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		DXE_CORE_ASSERT( !s_Instance, "Application already exists!" );
		s_Instance = this;
		
		m_Window = std::unique_ptr<Window>( Window::Create() );
		m_Window->SetEventCallback( BIND_EVENT_FN(OnEvent) );

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer( Layer* layer )
	{
		m_LayerStack.PushLayer( layer );
		layer->OnAttach();
	}

	void Application::PushOverlay( Layer* layer )
	{
		m_LayerStack.PushOverlay( layer );
		layer->OnAttach();
	}

	void Application::OnEvent( Event& e )
	{
		EventDispatcher dispatcher( e );
		dispatcher.Dispatch<WindowCloseEvent>( BIND_EVENT_FN( OnWindowClose ) );
		dispatcher.Dispatch<WindowResizeEvent>( BIND_EVENT_FN( OnWindowResize ) );

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent( e );
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Context->SetRenderTargets();
			m_Context->ClearScreen();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for ( Layer* layer : m_LayerStack )
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose( WindowCloseEvent& e )
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize( WindowResizeEvent& e )
	{
		m_Window->OnResize();
		return true;
	}
}