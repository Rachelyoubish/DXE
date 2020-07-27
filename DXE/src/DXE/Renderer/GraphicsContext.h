#pragma once

namespace DXE {

	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void ResizeContext() = 0;

		virtual ID3D11Device* GetD3D11Device() = 0;
		virtual ID3D11DeviceContext* GetD3D11DeviceContext() = 0;
	};
}