#include "scpch.h"
#include "WindowsInput.h"

#include "Seacrest/Application.h"

namespace Seacrest {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl( int keycode )
	{
		auto state = GetAsyncKeyState( keycode );

		return ( state & 0x8000 );
	}

	bool WindowsInput::IsMouseButtonPressedImpl( int button )
	{
		auto state = GetAsyncKeyState( button );

		return ( state & 0x8000 );
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		POINT pt;

		GetCursorPos( &pt );

		return { (float)pt.x, (float)pt.y };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();

		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();

		return y;
	}
}