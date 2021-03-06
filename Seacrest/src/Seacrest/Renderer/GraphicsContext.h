#pragma once

namespace Seacrest {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void ResizeContext() = 0;

		virtual void SetRenderTargets() = 0;
		virtual void ClearScreen() = 0;
		virtual void SetViewport( unsigned int width, unsigned int height ) = 0;

		virtual Microsoft::WRL::ComPtr<ID3D11Device> GetD3D11Device() = 0;
		virtual Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetD3D11DeviceContext() = 0;
		virtual Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetD3D11RTV() = 0;
	};
}