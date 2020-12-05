#include <Seacrest.h>

#include "imgui/imgui.h"

class ExampleLayer : public Seacrest::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	~ExampleLayer()
	{}

	void OnUpdate() override
	{
		// SEACREST_INFO( "ExampleLayer::Update" );

		if ( Seacrest::Input::IsKeyPressed( SEACREST_KEY_TAB ) )
			SEACREST_TRACE( "Tab key is pressed (poll)!" );

		// if ( Seacrest::Input::IsMouseButtonPressed( SEACREST_MOUSE_LBUTTON ) )
		//	SEACREST_TRACE( "Left Mouse Button pressed (poll)!" );

		//auto [x, y] = Seacrest::Input::GetMousePosition();
		//SEACREST_CORE_TRACE( "{0}, {1}", x, y );
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin( "Test" );
		ImGui::Text( "Seacrest: Hello World!" );
		ImGui::End();
	}

	void OnEvent( Seacrest::Event& event ) override
	{
		// SEACREST_TRACE( "{0}", event );
		if ( event.GetEventType() == Seacrest::EventType::KeyPressed )
		{
			Seacrest::KeyPressedEvent& e = ( Seacrest::KeyPressedEvent& ) event;
			if ( e.GetKeyCode() == SEACREST_KEY_TAB )
				SEACREST_TRACE( "Tab key is pressed (event)!" );

			// SEACREST_TRACE( "{0}", (char)e.GetKeyCode() );
		}

		if ( event.GetEventType() == Seacrest::EventType::MouseButtonPressed )
		{
			Seacrest::MouseButtonPressedEvent& e = ( Seacrest::MouseButtonPressedEvent& ) event;
			if ( e.GetMouseButton() == SEACREST_MOUSE_LBUTTON )
				SEACREST_TRACE( "Left Mouse Button pressed (event)!" );
		}
	}
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