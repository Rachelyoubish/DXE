#include <Seacrest.h>

#include "imgui/imgui.h"

class ExampleLayer : public Seacrest::Layer
{
public:
	ExampleLayer()
		: Layer( "Example" ), m_Camera( -1.6f, 1.6f, 0.9f, -0.9f ), m_CameraPosition( DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) ), m_SquarePosition(DirectX::XMMatrixIdentity())
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
			// Square. Set pos to .5f for 1 to 1 unit. 
			{  -0.5f,   0.5f, 1.0f, 0.8f, 0.8f, 1.0f },
			{   0.5f,   0.5f, 1.0f, 0.7f, 0.7f, 1.0f },
			{   0.5f,  -0.5f, 0.8f, 1.0f, 0.8f, 1.0f },
			{  -0.5f,  -0.5f, 0.8f, 0.8f, 0.8f, 1.0f },
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
		auto width = m_Window.GetWidth();
		auto height = m_Window.GetHeight();
		m_Context->SetViewport( width, height );

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

	void OnUpdate( Seacrest::Timestep ts ) override
	{
		#if SEACREST_DEBUG
			SEACREST_TRACE( "Delta time: {0}s, ({1}ms)", ts.GetSeconds(), ts.GetMilliSeconds() );
		#endif


		// Input
		if (Seacrest::Input::IsKeyPressed(SEACREST_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		}
		else if (Seacrest::Input::IsKeyPressed(SEACREST_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		}

		if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_UP ))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		}
		else if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		}

		if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_A ))
		{
			m_CameraRotation += m_CameraRotationSpeed * ts;
		}

		if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_D ))
		{
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_I ))
		{
			DirectX::XMFLOAT4X4 spF4;
			DirectX::XMStoreFloat4x4( &spF4, m_SquarePosition );
			spF4._42 += m_SquareMoveSpeed * ts;
			m_SquarePosition = DirectX::XMLoadFloat4x4( &spF4 );
		}
		else if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_K ))
		{
			DirectX::XMFLOAT4X4 spF4;
			DirectX::XMStoreFloat4x4( &spF4, m_SquarePosition );
			spF4._42 -= m_SquareMoveSpeed * ts;
			m_SquarePosition = DirectX::XMLoadFloat4x4( &spF4 );
		}

		if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_J ))
		{
			DirectX::XMFLOAT4X4 spF4;
			DirectX::XMStoreFloat4x4( &spF4, m_SquarePosition );
			spF4._41 -= m_SquareMoveSpeed * ts;
			m_SquarePosition = DirectX::XMLoadFloat4x4( &spF4 );
		}
		else if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_L ))
		{
			DirectX::XMFLOAT4X4 spF4;
			DirectX::XMStoreFloat4x4( &spF4, m_SquarePosition );
			spF4._41 += m_SquareMoveSpeed * ts;
			m_SquarePosition = DirectX::XMLoadFloat4x4( &spF4 );
		}

		if (Seacrest::Input::IsKeyPressed( SEACREST_KEY_BACK ))
		{
			m_CameraPosition = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
			m_CameraRotation = ( 0.0f );
		}

		Seacrest::RenderCommand::SetRenderTargets();
		Seacrest::RenderCommand::SetClearColor( { 0.16f, 0.16f, 0.16f, 1.0f } );
		Seacrest::RenderCommand::Clear();

		m_Camera.SetPosition( m_CameraPosition );
		m_Camera.SetRotation( m_CameraRotation );

		Seacrest::Renderer::BeginScene( m_Camera );
		/*Square Grid*/
		static DirectX::XMMATRIX scale = DirectX::XMMatrixScaling( 0.1f, 0.1f, 0.1f );
		
		DirectX::XMVECTOR redColor( { 0.8f, 0.2f, 0.3f, 1.0f } );
		DirectX::XMVECTOR blueColor( { 0.2f, 0.3f, 0.8f, 1.0f } );

		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 5; x++)
			{
				DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity() * scale;
				transform = transform * DirectX::XMMatrixTranslation( x * 0.11f, y * 0.11f, 0.0f );

				if (x % 2 == 0)
					m_SquareShader->UploadConstantFloat4( "cbColor", redColor );
				else
					m_SquareShader->UploadConstantFloat4( "cbColor", blueColor );
				Seacrest::Renderer::Submit( m_SquareShader, squareVB, squareIB, transform );
			}
		}
		//Seacrest::Renderer::Submit( m_SquareShader, squareVB, squareIB, m_SquarePosition );
		Seacrest::Renderer::Submit( m_Shader, vertexBuffer, indexBuffer);

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
	Seacrest::Window& m_Window = Seacrest::Application::Get().GetWindow();

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
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	DirectX::XMMATRIX m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;
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