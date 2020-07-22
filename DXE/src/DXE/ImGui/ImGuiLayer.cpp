#include "dxepch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platform/DirectX/ImGuiDirectXRenderer.h"

#include "DXE/Application.h"
#include "DXE/KeyCodes.h"

namespace DXE {

	static INT64                   g_Time = 0;
	static INT64                   g_TicksPerSecond = 0;

	static ID3D11Device*           g_pd3dDevice = NULL;
	static ID3D11DeviceContext*    g_pd3dDeviceContext = NULL;
	static IDXGISwapChain*         g_pSwapChain = NULL;
	static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

	void CreateRenderTarget()
	{
		ID3D11Texture2D* pBackBuffer;
		g_pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer );
		g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_mainRenderTargetView );
		pBackBuffer->Release();
	}

	void CleanupRenderTarget()
	{
		if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
	}

	HRESULT CreateDeviceD3D( HWND hWnd )
	{
		// Setup swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount = 2;
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
		if (D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext ) != S_OK)
			return E_FAIL;

		CreateRenderTarget();

		return S_OK;
	}

	ImGuiLayer::ImGuiLayer()
		: Layer( "ImGuiLayer" )
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		Application& app = Application::Get();
		auto window = static_cast<HWND>( Application::Get().GetWindow().GetNativeWindow() );

		if (CreateDeviceD3D( window ) < 0)
			DXE_ERROR( "Error creating the D3D object" );

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.KeyMap[ImGuiKey_Tab] = DXE_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = DXE_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = DXE_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = DXE_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = DXE_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = DXE_KEY_PRIOR;
		io.KeyMap[ImGuiKey_PageDown] = DXE_KEY_NEXT;
		io.KeyMap[ImGuiKey_Home] = DXE_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = DXE_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = DXE_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = DXE_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = DXE_KEY_BACK;
		io.KeyMap[ImGuiKey_Space] = DXE_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = DXE_KEY_RETURN;
		io.KeyMap[ImGuiKey_Escape] = DXE_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = DXE_KEY_A;
		io.KeyMap[ImGuiKey_C] = DXE_KEY_C;
		io.KeyMap[ImGuiKey_V] = DXE_KEY_V;
		io.KeyMap[ImGuiKey_X] = DXE_KEY_X;
		io.KeyMap[ImGuiKey_Y] = DXE_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = DXE_KEY_Z;

		ImGui_ImplDX11_Init( g_pd3dDevice, g_pd3dDeviceContext );
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2( app.GetWindow().GetWidth(), app.GetWindow().GetHeight() );

		INT64 current_time;
		::QueryPerformanceCounter( (LARGE_INTEGER*)&current_time );
		io.DeltaTime = (float)(current_time - g_Time) / g_TicksPerSecond;
		g_Time = current_time;

		ImGui_ImplDX11_NewFrame();
		ImGui::NewFrame();

		ImVec4 clear_color = ImVec4( 0.06f, 0.60f, 0.06f, 1.0f );
		static bool show = true;
		ImGui::ShowDemoWindow( &show );

		ImGui::Render();
		g_pd3dDeviceContext->OMSetRenderTargets( 1, &g_mainRenderTargetView, nullptr );
		g_pd3dDeviceContext->ClearRenderTargetView( g_mainRenderTargetView, (float*)&clear_color );
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

		g_pSwapChain->Present( 1, 0 );

		#if 0
			DXE_CORE_INFO( "{0}", g_Time );
		#endif 
	}

	void ImGuiLayer::OnEvent( Event& event )
	{
		EventDispatcher dispatcher( event );
		dispatcher.Dispatch<MouseButtonPressedEvent>( DXE_BIND_EVENT_FN( ImGuiLayer::OnMouseButtonPressedEvent ) );
		dispatcher.Dispatch<MouseButtonReleasedEvent>( DXE_BIND_EVENT_FN( ImGuiLayer::OnMouseButtonReleasedEvent ) );
		dispatcher.Dispatch<MouseMovedEvent>( DXE_BIND_EVENT_FN( ImGuiLayer::OnMouseMovedEvent ) );
		dispatcher.Dispatch<MouseScrolledEvent>( DXE_BIND_EVENT_FN( ImGuiLayer::OnMouseScrolledEvent ) );
		dispatcher.Dispatch<KeyPressedEvent>( DXE_BIND_EVENT_FN( ImGuiLayer::OnKeyPressedEvent ) );
		dispatcher.Dispatch<KeyTypedEvent>( DXE_BIND_EVENT_FN( ImGuiLayer::OnKeyTypedEvent ) );
		dispatcher.Dispatch<KeyReleasedEvent>( DXE_BIND_EVENT_FN( ImGuiLayer::OnKeyReleasedEvent ) );
		dispatcher.Dispatch<WindowResizeEvent>( DXE_BIND_EVENT_FN( ImGuiLayer::OnWindowResizeEvent ) );
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent( MouseButtonPressedEvent& e )
	{
		int button = { 0 };

		// Due to Win32 we need to remap the button for ImGui
		if (e.GetMouseButton() == 1) { button = 0; }
		if (e.GetMouseButton() == 2) { button = 1; }
		if (e.GetMouseButton() == 4) { button = 2; }

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[button] = true;

		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent( MouseButtonReleasedEvent& e )
	{
		int button = { 0 };

		// Due to Win32 we need to remap the button for ImGui
		if (e.GetMouseButton() == 1) { button = 0; }
		if (e.GetMouseButton() == 2) { button = 1; }
		if (e.GetMouseButton() == 4) { button = 2; }

		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[button] = false;

		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent( MouseMovedEvent& e )
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2( e.GetX(), e.GetY() );

		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent( MouseScrolledEvent& e )
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.GetDelta() / (float)WHEEL_DELTA;

		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent( KeyPressedEvent& e )
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = (::GetKeyState( DXE_KEY_CONTROL ) & 0x8000) != 0;
		io.KeyShift = (::GetKeyState( DXE_KEY_SHIFT ) & 0x8000) != 0;
		io.KeyAlt = (::GetKeyState( DXE_KEY_MENU ) & 0x8000) != 0;
		io.KeySuper = false;

		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent( KeyReleasedEvent& e )
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent( KeyTypedEvent& e )
	{
		ImGuiIO& io = ImGui::GetIO();

		int keycode = e.GetKeyCode();

		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter( (unsigned short)keycode );

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent( WindowResizeEvent& e )
	{
		CleanupRenderTarget();
		g_pSwapChain->ResizeBuffers( 0, (UINT)(e.GetWidth()), (UINT)(e.GetHeight()), DXGI_FORMAT_UNKNOWN, 0 );
		CreateRenderTarget();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2( e.GetWidth(), e.GetHeight() );
		io.DisplayFramebufferScale = ImVec2( 1.0f, 1.0f );

		return false;
	}
}