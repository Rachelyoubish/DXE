#pragma once

#include "Seacrest/Renderer/GraphicsContext.h"
#include <wrl.h>

namespace Seacrest {

	class D3DContext : public GraphicsContext
	{
	public:
		D3DContext(HWND windowHandle);
		~D3DContext() override;

		virtual void Init() override;
		virtual void SwapBuffers() override;
		virtual void ResizeContext() override;

		virtual void SetRenderTargets() override;
		virtual void ClearScreen() override;
		virtual void SetViewport( unsigned int width, unsigned int height ) override;

		virtual Microsoft::WRL::ComPtr<ID3D11Device> GetD3D11Device() override;
		virtual Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetD3D11DeviceContext() override;
		virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetD3D11RTV() override;
	private:
		void CreateRenderTarget();
		void CleanupRenderTarget();
		void SetTopology();

		void LogAdapterInfo();
	private:
		HWND m_WindowHandle;

		Microsoft::WRL::ComPtr<ID3D11Device> m_D3DDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_D3DDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
	};
}