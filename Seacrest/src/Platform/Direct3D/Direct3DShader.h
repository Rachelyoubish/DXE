#pragma once

#include "Seacrest/Renderer/Shader.h"
#include <string>
#include <DirectXMath.h>

namespace Seacrest {

	class Direct3DShader : public Shader
	{
	public:
		Direct3DShader( const std::string& vertexSrc, const std::string& pixelSrc );
		virtual ~Direct3DShader();
		
		virtual void Bind() const override;
		virtual void Unbind() const override;

		void UploadConstantFloat4( const std::string& name, const DirectX::XMVECTOR& value );
		void UploadConstantMat( const std::string& name, const DirectX::XMMATRIX& matrix );
		void UploadConstantTransform( const std::string& name, const DirectX::XMMATRIX& matrix );

		Microsoft::WRL::ComPtr<ID3DBlob> GetBlob() const { return pBlob; }
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBufferTransform;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBufferColor;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> reflector;
	};
}