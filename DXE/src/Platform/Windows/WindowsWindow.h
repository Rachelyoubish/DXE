#pragma once

#include "DXE/Window.h"
#include "DXE/Renderer/GraphicsContext.h"

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

		void OnResize() const override;
		
		inline virtual void* GetNativeWindow() const override { return m_Window; }
		inline virtual GraphicsContext* GetGraphicsContext() const override { return m_Context; }

		static LRESULT CALLBACK WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	private:
		virtual void Init( const WindowProps& props );
		virtual void Shutdown();
	private:
		HWND m_Window;
		HINSTANCE hInstance;
		static constexpr const char* pClassName = "DXE Class";
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height = 0;
			bool VSync = 0;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}