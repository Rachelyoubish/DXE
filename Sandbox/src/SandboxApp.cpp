#include <DXE.h>

class ExampleLayer : public DXE::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate() override
	{
		// DXE_INFO( "ExampleLayer::Update" );

		if ( DXE::Input::IsKeyPressed( DXE_KEY_TAB ) )
			DXE_TRACE( "Tab key is pressed (poll)!" );

		if ( DXE::Input::IsMouseButtonPressed( DXE_MOUSE_LBUTTON ) )
			DXE_TRACE( "Left Mouse Button pressed (poll)!" );
	}

	void OnEvent( DXE::Event& event ) override
	{
		// DXE_TRACE( "{0}", event );
		if ( event.GetEventType() == DXE::EventType::KeyPressed )
		{
			DXE::KeyPressedEvent& e = ( DXE::KeyPressedEvent& ) event;
			if ( e.GetKeyCode() == DXE_KEY_TAB )
				DXE_TRACE( "Tab key is pressed (event)!" );

			// DXE_TRACE( "{0}", (char)e.GetKeyCode() );
		}

		if ( event.GetEventType() == DXE::EventType::MouseButtonPressed )
		{
			DXE::MouseButtonPressedEvent& e = ( DXE::MouseButtonPressedEvent& ) event;
			if ( e.GetMouseButton() == DXE_MOUSE_LBUTTON )
				DXE_TRACE( "Left Mouse Button pressed (event)!" );
		}
	}
};

class Sandbox : public DXE::Application
{
public:
	Sandbox()
	{
		PushLayer( new ExampleLayer() );
		PushOverlay( new DXE::ImGuiLayer() );
	}

	~Sandbox()
	{

	}
};

DXE::Application* DXE::CreateApplication()
{
	return new Sandbox();
}