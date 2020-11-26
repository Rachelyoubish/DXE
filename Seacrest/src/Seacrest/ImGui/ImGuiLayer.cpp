#include "scpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#include "Seacrest/Application.h"
#include "Seacrest/Renderer/GraphicsContext.h"

#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_dx11.h"

#include "Seacrest/Log.h"

namespace Seacrest {

	ImGuiLayer::ImGuiLayer( bool renderDemo, bool renderConsole )
		: Layer( "ImGuiLayer" ), m_RenderDemo( renderDemo ), m_RenderConsole( renderConsole )
	{
	}

	void ImGuiLayer::OnAttach()
	{
		// ImGui_ImplWin32_EnableDpiAwareness();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		// io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
		// io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		
		ImGuiStyle& style = ImGui::GetStyle();
		if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		HWND window = static_cast<HWND>( Application::Get().GetWindow().GetNativeWindow() );

		ID3D11Device* pd3dDevice = app.GetWindow().GetGraphicsContext()->GetD3D11Device();
		ID3D11DeviceContext* pd3dDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init( window );
		ImGui_ImplDX11_Init( pd3dDevice, pd3dDeviceContext );
	}

	void ImGuiLayer::OnDetach()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2( (float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight() );

		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		ImGui::ShowDemoWindow( &m_RenderDemo );
		if (m_RenderConsole)
			ImGuiConsole::OnImGuiRender( &m_RenderConsole );
	}
}