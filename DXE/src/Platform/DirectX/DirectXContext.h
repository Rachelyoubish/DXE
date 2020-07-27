#pragma once

#include "DXE/Renderer/GraphicsContext.h"

namespace DXE {

	class D3DContext : public GraphicsContext
	{
	public:
		D3DContext(HWND windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void ResizeContext() override;

		void SetRenderTargets();
		void ClearScreen();

		virtual ID3D11Device* GetD3D11Device() override;
		virtual ID3D11DeviceContext* GetD3D11DeviceContext() override;

	private:
		void CreateRenderTarget();
		void CleanupRenderTarget();

		void LogAdapterInfo();
	private:
		HWND m_WindowHandle;
	};
}