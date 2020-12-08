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

		void UploadConstantBuffer( const std::string& name, DirectX::XMMATRIX matrix );

		Microsoft::WRL::ComPtr<ID3DBlob> GetBlob() { return pBlob; }
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;
	};
}