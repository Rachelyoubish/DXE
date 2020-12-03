#include "scpch.h"
#include "WindowsWindow.h"

#include "Seacrest/Events/ApplicationEvent.h"
#include "Seacrest/Events/MouseEvent.h"
#include "Seacrest/Events/KeyEvent.h"

#include "Platform/Direct3D/Direct3DContext.h"

#include "imgui.h"

#include "examples/imgui_impl_win32.h" // Temp: only here to fix DPI issues. 

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace Seacrest {

	static bool s_Win32Initialized = false;

	static void Win32ErrorCallback( int error, const char* description )
	{
		SEACREST_CORE_ERROR( "Win32 Error ({0}): {1}", error, description );
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
		ImGui_ImplWin32_EnableDpiAwareness();
		// Data
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		SEACREST_CORE_INFO( "Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height );

		// Gets instance of the current window
		hInstance = GetModuleHandle( 0 );
		// This struct holds information for the window class
		WNDCLASSEX wc = { 0 };

		// Clear out the window class for use
		SecureZeroMemory( &wc, sizeof( wc ) );

		// Filling in the struct with needed information
		wc.cbSize = sizeof( wc );
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof( WindowData* );
		wc.hInstance = hInstance;
		wc.hIcon = nullptr;
		wc.hCursor = LoadCursor( nullptr, IDC_ARROW );
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = pClassName;
		wc.hIconSm = nullptr;
		
		// Register the window class
		RegisterClassEx( &wc );

		// Calculate the size of the client area
		RECT wr = { 0 };
		wr.left = 100;
		wr.right = m_Data.Width + wr.left;
		wr.top = 100;
		wr.bottom = m_Data.Height + wr.top;
		AdjustWindowRect( &wr, WS_OVERLAPPEDWINDOW, FALSE );

		// Create the Window and use the result as the handle 
		m_Window = CreateWindowEx(
			0, pClassName,
			m_Data.Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, wc.hInstance, nullptr
		);

		if (!s_Win32Initialized)
		{
			SEACREST_CORE_ASSERT( m_Window, "Could not initialize Win32!" );

			s_Win32Initialized = true;
		}

		// Create DirectX Context
		m_Context = new D3DContext( m_Window );
		m_Context->Init();

		SetWindowLongPtr( m_Window, 0, (LONG_PTR)&m_Data );
		ShowWindow( m_Window, SW_SHOWDEFAULT );
	}

	void WindowsWindow::Shutdown()
	{
		DestroyWindow( m_Window );
	}

	void WindowsWindow::OnUpdate()
	{
		// Main loop of the window:

		// This struct holds Windows event messages
		MSG msg = { 0 };

		// Check to see if any messages are waiting in the queue
		while (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) > 0)
		{
			// Translate keystroke messages into the right format
			TranslateMessage( &msg );
			// Send the message to the WindowProc function
			DispatchMessage( &msg );
		}

		// Present next frame
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync( bool enabled )
	{
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::OnResize() const
	{
		m_Context->ResizeContext();
	}

	LRESULT CALLBACK WindowsWindow::WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		if ( ImGui_ImplWin32_WndProcHandler( hWnd, msg, wParam, lParam ) )
			return true;

		LRESULT result = NULL;

		WindowData& data = *(WindowData*)GetWindowLongPtr( hWnd, 0 );

		switch (msg)
		{
			case WM_SIZING:
			{
				RECT rect = *((PRECT)lParam);

				data.Width = (unsigned int)(rect.right - rect.left);
				data.Height = (unsigned int)(rect.bottom - rect.top);

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
			case WM_CHAR:
			{
				KeyTypedEvent event( static_cast<int>(wParam) );
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