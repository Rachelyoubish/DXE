#include <DXE.h>

class ExampleLayer : public DXE::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate() override
	{
		//DXE_INFO( "ExampleLayer::Update" );
	}

	void OnEvent( DXE::Event& event ) override
	{
		DXE_TRACE( "{0}", event );
	}
};

class Sandbox : public DXE::Application
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

DXE::Application* DXE::CreateApplication()
{
	return new Sandbox();
}