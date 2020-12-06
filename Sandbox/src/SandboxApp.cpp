#include <Seacrest.h>

#include "imgui/imgui.h"

class ExampleLayer : public Seacrest::Layer
{
public:
	ExampleLayer()
		: Layer( "Example" ), m_Camera( -1.6f, 1.6f, 0.9f, -0.9f ), m_CameraPosition( DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) )
	{
		m_Context = Seacrest::Application::Get().GetWindow().GetGraphicsContext();

		m_Device = Seacrest::Application::Get().GetWindow().GetGraphicsContext()->GetD3D11Device();
		m_DeviceContext = Seacrest::Application::Get().GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();

		m_InputLayout.reset( Seacrest::InputLayout::Create() );

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

		vertexBuffer.reset( Seacrest::VertexBuffer::Create( vertices, sizeof( Vertex ), sizeList ) );

		// Buffer Layout setup here for InputLayout
		// to take in once shaders are set later.
		// very linear, could be improved. 
		Seacrest::BufferLayout layout = {
			{ Seacrest::ShaderDataType::Float2, "Position" },
			{ Seacrest::ShaderDataType::Float4, "Color" },
		};
		vertexBuffer->SetLayout( layout );


		// Create index buffer.
		unsigned short indices[] =
		{
			// Triangle. 
			0, 1, 2,
		};

		unsigned short indicesList = ARRAYSIZE( indices );
		indexBuffer.reset( Seacrest::IndexBuffer::Create( indices, sizeof( indices ), indicesList ) );

		// Square.
		m_SquareInput.reset( Seacrest::InputLayout::Create() );

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
			{  -0.75f,   0.75f, 1.0f, 0.8f, 0.8f, 1.0f },
			{   0.75f,   0.75f, 1.0f, 0.7f, 0.7f, 1.0f },
			{   0.75f,  -0.75f, 0.8f, 1.0f, 0.8f, 1.0f },
			{  -0.75f,  -0.75f, 0.8f, 0.8f, 0.8f, 1.0f },
		};

		squareVB.reset( Seacrest::VertexBuffer::Create( squareVertices, sizeof( SquareVertex ), ARRAYSIZE( squareVertices ) ) );
		squareVB->SetLayout( {
			{ Seacrest::ShaderDataType::Float2, "Position" },
			{ Seacrest::ShaderDataType::Float4, "Color" },
			} );

		// Create index buffer.
		unsigned short squareIndices[] =
		{
			// Square.
			0, 1, 2,
			0, 2, 3,
		};
		squareIB.reset( Seacrest::IndexBuffer::Create( squareIndices, sizeof( squareIndices ), ARRAYSIZE( squareIndices ) ) );

		// Trying to set D3D viewport abstractly. 
		// Could be handled better, sorry. ;^]
		//auto width = m_Window->GetWidth();
		//auto height = m_Window->GetHeight();
		m_Context->SetViewport( 1600, 900 );

		m_Shader.reset( new Seacrest::Shader( "VertexShader.cso", "PixelShader.cso" ) );
		// Blob set after shader setup to retain shader info. 
		auto Blob = m_Shader->GetBlob();
		// Now InputLayout can properly read Blob info. 
		// (This is admittedly a symptom of D3D).
		m_InputLayout->AddVertexBuffer( vertexBuffer, Blob.Get() );
		m_InputLayout->Bind();

		m_SquareShader.reset( new Seacrest::Shader( "SquareVS.cso", "SquarePS.cso" ) );
		m_SquareInput->AddVertexBuffer( squareVB, Blob.Get() );
		m_SquareInput->Bind();
	}

	~ExampleLayer()
	{}

	void OnUpdate() override
	{

		if (Seacrest::Input::IsKeyPressed(SEACREST_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (Seacrest::Input::IsKeyPressed(SEACREST_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed;
		}

		if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_UP ))
		{
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		else if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}

		if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_A ))
		{
			m_CameraRotation += m_CameraRotationSpeed;
		}

		if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_D ))
		{
			m_CameraRotation -= m_CameraRotationSpeed;
		}

		Seacrest::RenderCommand::SetRenderTargets();
		Seacrest::RenderCommand::SetClearColor( { 0.16f, 0.16f, 0.16f, 1.0f } );
		Seacrest::RenderCommand::Clear();

		m_Camera.SetPosition( m_CameraPosition );
		m_Camera.SetRotation( m_CameraRotation );

		Seacrest::Renderer::BeginScene( m_Camera );

		Seacrest::Renderer::Submit( m_SquareShader, squareVB, squareIB );
		Seacrest::Renderer::Submit( m_Shader, vertexBuffer, indexBuffer );

		Seacrest::Renderer::EndScene();

		#ifdef SEACREST_DEBUG
		ReportLiveObjects();
		#endif
	}

	virtual void OnImGuiRender() override
	{
	}

	void OnEvent( Seacrest::Event& event ) override
	{
	}

	void ReportLiveObjects()
	{
		ID3D11Debug* d3d11Debug;
		m_Device->QueryInterface( IID_PPV_ARGS( &d3d11Debug ) );

		d3d11Debug->ReportLiveDeviceObjects( D3D11_RLDO_IGNORE_INTERNAL );
		d3d11Debug->Release();
	}

private:
	Seacrest::GraphicsContext* m_Context;

	std::shared_ptr<Seacrest::Shader> m_Shader;
	std::shared_ptr<Seacrest::VertexBuffer> vertexBuffer;
	std::shared_ptr<Seacrest::IndexBuffer> indexBuffer;
	std::shared_ptr<Seacrest::InputLayout> m_InputLayout;

	std::shared_ptr<Seacrest::Shader> m_SquareShader;
	std::shared_ptr<Seacrest::VertexBuffer> squareVB;
	std::shared_ptr<Seacrest::IndexBuffer> squareIB;
	std::shared_ptr<Seacrest::InputLayout> m_SquareInput;

	Seacrest::OrthographicCamera m_Camera;
	DirectX::XMFLOAT3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.1f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 2.0f;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
};

class Sandbox : public Seacrest::Application
{
public:
	Sandbox()
	{
		PushLayer( new ExampleLayer() );
	}

	~Sandbox()
	{

	}
};

Seacrest::Application* Seacrest::CreateApplication()
{
	return new Sandbox();
}