#include "dxepch.h"
#include "WindowsWindow.h"

#include "DXE/Events/ApplicationEvent.h"
#include "DXE/Events/MouseEvent.h"
#include "DXE/Events/KeyEvent.h"

namespace DXE {

	static bool s_Win32Initialized = false;

	static void Win32ErrorCallback( int error, const char* description )
	{
		DXE_CORE_ERROR( "Win32 Error ({0}): {1}", error, description );
	}

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

		hInstance = GetModuleHandle( 0 );

		WNDCLASSEX wc = {};
		wc.cbSize = sizeof( wc );
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof( WindowData* );
		wc.hInstance = hInstance;
		wc.hIcon = nullptr;
		wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
		wc.hbrBackground = (HBRUSH)GetStockObject( GRAY_BRUSH );
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = pClassName;
		wc.hIconSm = nullptr;

		RegisterClassEx( &wc );

		RECT wr;
		wr.left = 100;
		wr.right = props.Width + wr.left;
		wr.top = 100;
		wr.bottom = props.Height + wr.top;
		AdjustWindowRect( &wr, WS_OVERLAPPEDWINDOW, FALSE );

		m_Window = CreateWindowEx(
			0, pClassName,
			props.Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, hInstance, nullptr
		);

		if (!s_Win32Initialized)
		{
			DXE_CORE_ASSERT( (m_Window == nullptr), "Could not initialize Win32!" );

			s_Win32Initialized = true;
		}

		SetWindowLongPtr( m_Window, 0, (LONG_PTR)&m_Data );
		ShowWindow( m_Window, SW_SHOW );
	}

	void WindowsWindow::Shutdown()
	{
		DestroyWindow( m_Window );
	}

	void WindowsWindow::OnUpdate()
	{
		// DXE_CORE_TRACE( "Frame update" );

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
		LRESULT result = NULL;

		WindowData& data = *(WindowData*)GetWindowLongPtr( hWnd, 0 );

		switch (msg)
		{
			case WM_SIZING:
			{
				RECT rect = *((PRECT)lParam);

				WindowResizeEvent event( (unsigned int)(rect.right - rect.left), (unsigned int)(rect.bottom - rect.top) );
				data.EventCallback( event );
				break;
			}
			case WM_CLOSE:
			//case WM_DESTROY:
			{
				WindowCloseEvent event;
				data.EventCallback( event );
				break;
			}
			case WM_KEYDOWN:
			{
				int repeatCount = (lParam & 0xffff);

				KeyPressedEvent event( static_cast<int>(wParam), repeatCount );
				data.EventCallback( event );
				break;
			}
			case WM_KEYUP:
			{
				KeyReleasedEvent event( static_cast<int>(wParam) );
				data.EventCallback( event );
				break;
			}
			case WM_MOUSEMOVE:
			{
				MouseMovedEvent event( (float)GET_X_LPARAM( lParam ), (float)GET_Y_LPARAM( lParam ) );
				data.EventCallback( event );
				break;
			}
			case WM_MOUSEWHEEL:
			{
				MouseScrolledEvent event( GET_WHEEL_DELTA_WPARAM( wParam ) );
				data.EventCallback( event );
				break;
			}
			case WM_LBUTTONDOWN:
			{
				MouseButtonPressedEvent event( VK_LBUTTON );
				data.EventCallback( event );
				break;
			}
			case WM_LBUTTONUP:
			{
				MouseButtonReleasedEvent event( VK_LBUTTON );
				data.EventCallback( event );
				break;
			}
			case WM_MBUTTONDOWN:
			{
				MouseButtonPressedEvent event( VK_MBUTTON );
				data.EventCallback( event );
				break;
			}
			case WM_MBUTTONUP:
			{
				MouseButtonReleasedEvent event( VK_MBUTTON );
				data.EventCallback( event );
				break;
			}
			case WM_RBUTTONDOWN:
			{
				MouseButtonPressedEvent event( VK_RBUTTON );
				data.EventCallback( event );
				break;
			}
			case WM_RBUTTONUP:
			{
				MouseButtonReleasedEvent event( VK_RBUTTON );
				data.EventCallback( event );
				break;
			}
			default:
				result = DefWindowProc( hWnd, msg, wParam, lParam );
		}
		return result;
	}
}