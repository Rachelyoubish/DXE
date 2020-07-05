#pragma once

#include "DXE/Window.h"

namespace DXE {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow( const WindowProps& props );
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback( const EventCallbackFn& callback ) override { m_Data.EventCallback = callback; }
		void SetVSync( bool enabled ) override;
		bool IsVSync() const override;

		inline HWND GetWindowHandle() const override { return m_Window; }

		static LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	private:
		virtual void Init( const WindowProps& props );
		virtual void Shutdown();
	private:
		HWND m_Window;
		HINSTANCE hInstance;
		static constexpr const char* pClassName = "DXE Class";

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}