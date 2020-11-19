#include "dxepch.h"
#include "Application.h"

#include "DXE/Log.h"

#include "Input.h"

// TEMP includes
#include <wrl.h>
#include <d3dcompiler.h>

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

		m_Context = Get().GetWindow().GetGraphicsContext();

		// TEMP: (Just getting a triangle on the screen, to be moved).
		m_Device = Get().GetWindow().GetGraphicsContext()->GetD3D11Device();
		m_DeviceContext = Get().GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
		
		struct Vertex
		{
			float x;
			float y;
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};
		
		// Create vertex buffer (1 2d triangle at the center of the screen).
		const Vertex vertices[] =
		{
			{  0.0f,  0.5f, 255, 0, 0, 0 },
			{  0.5f, -0.5f, 0, 255, 0, 0 },
			{ -0.5f, -0.5f, 0, 0, 255, 0 },
		};
		
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		
		D3D11_BUFFER_DESC bd = {};
		
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof( vertices );
		bd.StructureByteStride = sizeof( Vertex );
		
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;
		
		m_Device->CreateBuffer( &bd, &sd, &pVertexBuffer );
		
		// Bind vertex buffer to pipeline
		const UINT stride = sizeof( Vertex );
		const UINT offset = 0u;
		m_DeviceContext->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );
		
		// old shader code was here.
		
		// Bind render target.
		// SetRenderTargets();
		// However, in the context of the current setup, render targets
		// are already set every frame, do I really need to duplicate
		// setting them in this function again? 
		
		// Set primitive topology to triangle list (groups of 3 vertices).
		m_DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		
		// Configure viewport.
		D3D11_VIEWPORT vp{ 0 };
		// Temp: Make sure width and height match that 
		// which is described in window constructor.
		vp.Width = (float)m_Window->GetWidth();
		vp.Height = (float)m_Window->GetHeight();
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_DeviceContext->RSSetViewports( 1u, &vp );
		
		// m_DeviceContext->Draw( (UINT)std::size( vertices ), 0u );

		// Stores the amount of vertices for the Draw command.
		m_VertexBuffer = (UINT)std::size( vertices );

		m_Shader.reset( new Shader( "VertexShader.cso", "PixelShader.cso", m_Device, m_DeviceContext ) );
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

			m_Shader->Bind();
			m_DeviceContext->Draw( m_VertexBuffer, 0u );

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