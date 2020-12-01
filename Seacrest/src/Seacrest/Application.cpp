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
			// Triangle.
			{  0.0f,  0.5f, 0.8f, 0.2f, 0.8f, 1.0f },
			{  0.5f, -0.5f, 0.2f, 0.3f, 0.8f, 1.0f },
			{ -0.5f, -0.5f, 0.8f, 0.8f, 0.2f, 1.0f },
		};

		// vertices[0].color.g = 255;

		UINT sizeList = ARRAYSIZE( vertices );

		// std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create( vertices, sizeof( Vertex ), sizeList ) );

		// Buffer Layout setup here for InputLayout
		// to take in once shaders are set later.
		// very linear, could be improved. 
		BufferLayout layout = {
			{ ShaderDataType::Float2, "Position" },
			{ ShaderDataType::Float4, "Color" },
		};
		vertexBuffer->SetLayout( layout );

		
		// Create index buffer.
		unsigned short indices[] =
		{
			// Triangle. 
			0, 1, 2,
		};
		
		unsigned short indicesList = ARRAYSIZE( indices );
		// std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset( IndexBuffer::Create( indices, sizeof( indices ), indicesList ) );

		// Square.
		// m_SquareInput.reset( InputLayout::Create() );

		struct SquareVertex
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

		SquareVertex squareVertices[] =
		{
			// Square.
			{  -0.7f,   0.7f, 0.2f, 0.8f, 0.8f, 1.0f },
			{   0.7f,   0.7f, 0.2f, 0.7f, 0.7f, 1.0f },
			{   0.7f,  -0.7f, 0.8f, 0.2f, 0.8f, 1.0f },
			{  -0.7f,  -0.7f, 0.8f, 0.8f, 0.8f, 1.0f },
		};

		//std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset( VertexBuffer::Create( squareVertices, sizeof( SquareVertex ), ARRAYSIZE( squareVertices ) ) );
		squareVB->SetLayout( {
			{ ShaderDataType::Float2, "Position" },
			{ ShaderDataType::Float4, "Color" },
			} );

		// Create index buffer.
		unsigned short squareIndices[] =
		{
			// Square.
			0, 1, 2,
			0, 2, 3,
		};
		// std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset( IndexBuffer::Create( squareIndices, sizeof( squareIndices ), ARRAYSIZE( squareIndices) ) );


		// Trying to set D3D viewport abstractly. 
		// Could be handled better, sorry. ;^]
		auto width = m_Window->GetWidth();
		auto height = m_Window->GetHeight();
		m_Context->SetViewport( width, height ); 

		m_Shader.reset( new Shader( "VertexShader.cso", "PixelShader.cso") );
		// Blob set after shader setup to retain shader info. 
		auto Blob = m_Shader->GetBlob();


		vertexBuffer->Bind();

		vertexBuffer->GetLayout();

		// Input (vertex) layout (2D position only).
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Color",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		m_InputLayout.reset( InputLayout::Create(ied, Blob ));


		// Now InputLayout can properly read Blob info. 
		// (This is admittedly a symptom of D3D).
		m_InputLayout->AddVertexBuffer( vertexBuffer, Blob  );
		m_InputLayout->SetIndexBuffer( indexBuffer );


		squareVB->Bind();
		
		squareVB->GetLayout();
		// Input (vertex) layout (2D position only).
		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied2 =
		{
			{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		m_SquareShader.reset( new Shader( "SquareVS.cso", "SquarePS.cso" ) );
		auto Blob2 = m_SquareShader->GetBlob();
		//m_SquareInput->AddVertexBuffer( squareVB, Blob2 );
		m_SquareInput.reset( InputLayout::Create(ied2, Blob2) );
		m_SquareInput->SetIndexBuffer( squareIB );
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

			m_SquareShader->Bind();
			m_SquareInput->Bind();
			squareVB->Bind();
			squareIB->Bind();
			m_DeviceContext->DrawIndexed( m_SquareInput->GetIndexBuffer()->GetCount(), 0u, 0u );

			m_Shader->Bind();
			m_InputLayout->Bind();
			vertexBuffer->Bind();
			indexBuffer->Bind();
			m_DeviceContext->DrawIndexed( m_InputLayout->GetIndexBuffer()->GetCount(), 0u, 0u );

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