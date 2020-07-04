#include "dxepch.h"
#include "WindowsWindow.h"

namespace DXE {

	static bool s_Win32Initialized = false;

	Window* Window::Create( const WindowProps& props )
	{
		return new WindowsWindow( props );
	}

	WindowsWindow::WindowsWindow( const WindowProps& props )
	{
		Init( props );
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init( const WindowProps& props )
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		DXE_CORE_INFO( "Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height );

		WNDCLASSEX wc = {};
		wc.cbSize = sizeof( wc );
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = pClassName;
		wc.hIconSm = nullptr;

		RegisterClassEx( &wc );

		m_Window = CreateWindowEx(
			0, pClassName,
			props.Title.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, (int)props.Width, (int)props.Height,
			nullptr, nullptr, hInstance, nullptr
		);

		if (!s_Win32Initialized)
		{
			DXE_CORE_ASSERT( (m_Window == nullptr), "Could not initialize Win32!" );
		}

		ShowWindow( m_Window, SW_SHOW );
	}

	void WindowsWindow::Shutdown()
	{
		DestroyWindow( m_Window );
	}

	void WindowsWindow::OnUpdate()
	{
		// Bug: mutex something something...
		// Likely need to do events to make this mesh well. 
		DXE_CORE_TRACE( "Frame update" );

		MSG msg;

		while (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

	}

	void WindowsWindow::SetVSync( bool enabled )
	{
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	LRESULT CALLBACK WindowsWindow::WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{

		switch (msg)
		{
			case WM_CLOSE:
				DXE_CORE_INFO("WindowClosed");
				break;
		}
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
}