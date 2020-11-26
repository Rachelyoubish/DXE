#pragma once

#include "scpch.h"

#include "Seacrest/Core.h"
#include "Seacrest/Events/Event.h"

#include "Seacrest/Renderer/GraphicsContext.h"

namespace Seacrest {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Seacrest Engine",
					unsigned int width = 1600,
					unsigned int height = 900)
			: Title(title), Width(width), Height(height)
		{}
	};

	// Interface representing a desktop system based Window.

	class SEACREST_API Window
	{
	public:
		using EventCallbackFn = std::function<void( Event& )>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback( const EventCallbackFn& callback ) = 0;
		virtual void SetVSync( bool enabled ) = 0;
		virtual bool IsVSync() const = 0;

		virtual void OnResize() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual GraphicsContext* GetGraphicsContext() const = 0;

		static Window* Create( const WindowProps& props = WindowProps() );
	};
}