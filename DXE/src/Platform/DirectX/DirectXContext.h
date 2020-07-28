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

		virtual void SetRenderTargets() override;
		virtual void ClearScreen() override;

		virtual ID3D11Device* GetD3D11Device() override;
		virtual ID3D11DeviceContext* GetD3D11DeviceContext() override;

	private:
		void CreateRenderTarget();
		void CleanupRenderTarget();

		void LogAdapterInfo();
	private:
		HWND m_WindowHandle;

		ID3D11Device* m_D3DDevice = nullptr;
		ID3D11DeviceContext* m_D3DDeviceContext = nullptr;
		IDXGISwapChain* m_SwapChain = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;
	};
}