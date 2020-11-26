#include "scpch.h"
#include "Application.h"

#include "Seacrest/Log.h"

#include "Input.h"

namespace Seacrest {

#define BIND_EVENT_FN(x) std::bind( &Application::x, this, std::placeholders::_1 )

	Application* Application::s_Instance = nullptr;

	static DXGI_FORMAT ShaderDataTypeToDirect3DBaseType( ShaderDataType type )
	{
		switch (type)
		{
			case Seacrest::ShaderDataType::Float:		return DXGI_FORMAT_R32_FLOAT;
			case Seacrest::ShaderDataType::Float2:		return DXGI_FORMAT_R32G32_FLOAT;
			case Seacrest::ShaderDataType::Float3:		return DXGI_FORMAT_R32G32B32_FLOAT;
			case Seacrest::ShaderDataType::Float4:		return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case Seacrest::ShaderDataType::Int:		    return DXGI_FORMAT_R32_UINT;
			case Seacrest::ShaderDataType::Int2:		return DXGI_FORMAT_R32G32_UINT;
			case Seacrest::ShaderDataType::Int3:		return DXGI_FORMAT_R32G32B32_UINT;
			case Seacrest::ShaderDataType::Int4:		return DXGI_FORMAT_R32G32B32A32_UINT;
		}

		SEACREST_CORE_ASSERT( false, "Unkown ShaderDataType!" );
		return DXGI_FORMAT_UNKNOWN;
	}

	Application::Application()
	{
		SEACREST_CORE_ASSERT( !s_Instance, "Application already exists!" );
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
			struct
			{
				float x;
				float y;
			} pos;
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} color;
		};

		Vertex vertices[] =
		{
			{  0.0f,  0.5f, 0.8f, 0.2f, 0.8f, 1.0f },
			{  0.5f, -0.5f, 0.2f, 0.3f, 0.8f, 1.0f },
			{ -0.5f, -0.5f, 0.8f, 0.8f, 0.2f, 1.0f },
		};

		// vertices[0].color.g = 255;

		UINT sizeList = ARRAYSIZE( vertices );

		m_VertexBuffer.reset(VertexBuffer::Create( vertices, sizeof( Vertex ), sizeList ) );

		// To prevent outside access to layout, place in its own local block. 
		{
			BufferLayout layout = {
				{ ShaderDataType::Float2, "Position" },
				{ ShaderDataType::Float4, "Color" },
			};
			m_VertexBuffer->SetLayout( layout );
		}

		uint32_t index = 0;
		const auto& layout = m_VertexBuffer->GetLayout();

		// Input (vertex) layout (2D position only).
		D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc = new D3D11_INPUT_ELEMENT_DESC[layout.GetElements().size()];
		for (const auto& element : layout )
		{
			inputLayoutDesc[index].SemanticName = element.Name.c_str();
			inputLayoutDesc[index].SemanticIndex = 0;
			inputLayoutDesc[index].Format = ShaderDataTypeToDirect3DBaseType( element.Type );
			inputLayoutDesc[index].InputSlot = 0;
			inputLayoutDesc[index].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			inputLayoutDesc[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			inputLayoutDesc[index].InstanceDataStepRate = 0;

			index++;
		}
		
		// Create index buffer.
		unsigned short indices[] =
		{
			0, 1, 2,
		};
		
		unsigned short indicesList = ARRAYSIZE( indices );

		m_IndexBuffer.reset( IndexBuffer::Create( indices, sizeof( indices ), indicesList ) );
		
		// Set primitive topology to triangle list (groups of 3 vertices).
		m_DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
		
		// Configure viewport.
		D3D11_VIEWPORT vp{ 0 };
		// // Temp: Make sure width and height match that 
		// // which is described in window constructor.
		vp.Width = (float)m_Window->GetWidth();
		vp.Height = (float)m_Window->GetHeight();
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_DeviceContext->RSSetViewports( 1u, &vp );
		
		// m_DeviceContext->Draw( (UINT)std::size( vertices ), 0u );

		// Stores the amount of vertices for the Draw command.
		m_Vertex = (UINT)std::size( vertices );
		m_Index = (UINT)std::size( indices );

		m_Shader.reset( new Shader( "VertexShader.cso", "PixelShader.cso") );
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob = m_Shader->GetBlob();

		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		m_Device->CreateInputLayout(
			inputLayoutDesc, (UINT)layout.GetElements().size(),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		);

		// Bind vertex layout.
		m_DeviceContext->IASetInputLayout( pInputLayout.Get() );

		delete[] inputLayoutDesc;
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

			// Binding the Vertex, Index, and Input Layout here instead may be something to do...
			// Especially when constant buffers come around, I assume? 
			// > Set Input
			// > Bind Vertex
			// > Bind Index

			m_Shader->Bind();
			// m_DeviceContext->Draw( m_Vertex, 0 );
			m_DeviceContext->DrawIndexed( m_IndexBuffer->GetCount(), 0u, 0u );

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