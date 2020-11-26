#pragma once

#include <string>
#include <wrl.h>

namespace Seacrest {
	class Shader
	{
	public:
		Shader( const std::string& vertexSrc, const std::string& pixelSrc );
		~Shader();
		void Bind();
		void Unbind();
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	};
}