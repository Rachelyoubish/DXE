#include <Seacrest.h>

#include "Platform/Direct3D/Direct3DShader.h"

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

		m_VertexBuffer.reset( Seacrest::VertexBuffer::Create( vertices, sizeof( Vertex ), sizeList ) );

		// Buffer Layout setup here for InputLayout
		// to take in once shaders are set later.
		// very linear, could be improved. 
		Seacrest::BufferLayout layout = {
			{ Seacrest::ShaderDataType::Float2, "Position" },
			{ Seacrest::ShaderDataType::Float4, "Color" },
		};
		m_VertexBuffer->SetLayout( layout );


		// Create index buffer.
		unsigned short indices[] =
		{
			// Triangle. 
			0, 1, 2,
		};

		unsigned short indicesList = ARRAYSIZE( indices );
		m_IndexBuffer.reset( Seacrest::IndexBuffer::Create( indices, sizeof( indices ), indicesList ) );

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
			struct
			{
				float u;
				float v;
			} tex;
		};

		SquareVertex squareVertices[] =
		{
			// Square. Set pos to .5f for 1 to 1 unit. 
			{ -0.5f,   0.5f, 1.0f, 0.8f, 0.8f, 1.0f, 0.0f, 0.0f }, // Per struct above: pos, color, texCoord
			{  0.5f,   0.5f, 1.0f, 0.7f, 0.7f, 1.0f, 1.0f, 0.0f },
			{  0.5f,  -0.5f, 0.8f, 1.0f, 0.8f, 1.0f, 1.0f, 1.0f },
			{ -0.5f,  -0.5f, 0.8f, 0.8f, 0.8f, 1.0f, 0.0f, 1.0f },
		};

		m_SquareVB.reset( Seacrest::VertexBuffer::Create( squareVertices, sizeof( SquareVertex ), ARRAYSIZE( squareVertices ) ) );
		m_SquareVB->SetLayout( {
			{ Seacrest::ShaderDataType::Float2, "Position" },
			{ Seacrest::ShaderDataType::Float4, "Color" },
			{ Seacrest::ShaderDataType::Float2, "TexCoord" },
			} );

		// Create index buffer.
		unsigned short squareIndices[] =
		{
			// Square.
			0, 1, 2,
			0, 2, 3,
		};
		m_SquareIB.reset( Seacrest::IndexBuffer::Create( squareIndices, sizeof( squareIndices ), ARRAYSIZE( squareIndices ) ) );

		// Trying to set D3D viewport abstractly. 
		// Could be handled better, sorry. ;^]
		auto width = m_Window.GetWidth();
		auto height = m_Window.GetHeight();
		m_Context->SetViewport( width, height );

		m_Shader.reset( Seacrest::Shader::Create( "VertexShader.cso", "PixelShader.cso" ) );
		
		// Blob set after shader setup to retain shader info. 
		auto Blob = std::dynamic_pointer_cast<Seacrest::Direct3DShader>( m_Shader )->GetBlob();
		
		// Now InputLayout can properly read Blob info. 
		// (This is admittedly a symptom of D3D).
		m_InputLayout->AddVertexBuffer( m_VertexBuffer, Blob.Get() );
		m_InputLayout->Bind();

		m_FlatColorShader.reset( Seacrest::Shader::Create( "FlatColorVS.cso", "FlatColorPS.cso" ) );
		m_SquareInput->AddVertexBuffer( m_SquareVB, Blob.Get() );
		m_SquareInput->Bind();

		m_TextureShader.reset( Seacrest::Shader::Create( "TextureVS.cso", "TexturePS.cso" ) );
		m_SquareInput->AddVertexBuffer( m_SquareVB, Blob.Get() );
		m_SquareInput->Bind();

		// Path seems to be relative to WIC's file location, 
		// we have to jump out of nested vendor folder and into Sandbox. 
		m_Texture = Seacrest::Texture2D::Create("../../../Sandbox/assets/textures/Checkerboard.png");
		m_RomulusTexture = Seacrest::Texture2D::Create("../../../Sandbox/assets/textures/RIVBlack.png");
	}

	~ExampleLayer()
	{}

	void OnUpdate( Seacrest::Timestep ts ) override
	{
		#if 0
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

		std::dynamic_pointer_cast<Seacrest::Direct3DShader>( m_FlatColorShader )->Bind();
		std::dynamic_pointer_cast<Seacrest::Direct3DShader>( m_FlatColorShader )->UploadConstantFloat4("cbColor", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity() * scale;
				transform = transform * DirectX::XMMatrixTranslation( x * 0.11f, y * 0.11f, 0.0f );

				Seacrest::Renderer::Submit( m_FlatColorShader, m_SquareVB, m_SquareIB, transform );
			}
		}
		
		m_Texture->Bind();
		Seacrest::Renderer::Submit( m_TextureShader, m_SquareVB, m_SquareIB, DirectX::XMMatrixScaling( 1.5f, 1.5f, 1.5f ) );
		m_RomulusTexture->Bind();
		Seacrest::Renderer::Submit( m_TextureShader, m_SquareVB, m_SquareIB, DirectX::XMMatrixScaling( 1.5f, 1.5f, 1.5f ) );
		//Seacrest::Renderer::Submit( m_Shader, m_SquareVB, m_SquareIB, DirectX::XMMatrixTranslation( -0.5f, -0.3f, 0.0f ) );
		//Seacrest::Renderer::Submit( m_Shader, m_VertexBuffer, m_IndexBuffer);

		Seacrest::Renderer::EndScene();

		#ifdef SEACREST_DEBUG
		ReportLiveObjects();
		#endif
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin( "Settings" );
		ImGui::ColorEdit4( "Square Color", m_SquareColor.m128_f32 );
		ImGui::End();
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
	Seacrest::Ref<Seacrest::Shader> m_Shader;
	Seacrest::Ref<Seacrest::VertexBuffer> m_VertexBuffer;
	Seacrest::Ref<Seacrest::IndexBuffer> m_IndexBuffer;
	Seacrest::Ref<Seacrest::InputLayout> m_InputLayout;

	Seacrest::Ref<Seacrest::Shader> m_FlatColorShader, m_TextureShader;
	Seacrest::Ref<Seacrest::VertexBuffer> m_SquareVB;
	Seacrest::Ref<Seacrest::IndexBuffer> m_SquareIB;
	Seacrest::Ref<Seacrest::InputLayout> m_SquareInput;

	Seacrest::Ref<Seacrest::Texture2D> m_Texture, m_RomulusTexture;

	Seacrest::OrthographicCamera m_Camera;
	DirectX::XMFLOAT3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	DirectX::XMVECTOR m_SquareColor = DirectX::XMVectorSet(0.2f, 0.3f, 0.8f, 1.0f );
private:
	Seacrest::GraphicsContext* m_Context;
	Seacrest::Window& m_Window = Seacrest::Application::Get().GetWindow();
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