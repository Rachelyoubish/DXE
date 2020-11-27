#include "scpch.h"
#include "Shader.h"
#include "Seacrest/Application.h"
#include <d3dcompiler.h>

namespace Seacrest {
	
	Shader::Shader( const std::string& vertexSrc, const std::string& pixelSrc )
	{
		Application& app = Application::Get();
		m_Device = app.GetWindow().GetGraphicsContext()->GetD3D11Device();
		m_DeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();

		// String conversion for D3DReadFileToBlob.
		std::wstring ws;
		ws.assign( pixelSrc.begin(), pixelSrc.end() );
		// Get temp LPCWSTR (said to be safe). 
		LPCWSTR ps = ws.c_str();

		// Create pixel shader.
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		// PS sets and uses Blob first so vertex shader keeps relevant 
		// Blob info once released from PS (and given to VS, handled automatically by COM).
		// Remember: & releases then gets the pointer address.
		D3DReadFileToBlob( ps, &pBlob );
		m_Device->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader );

		// Bind pixel shader.
		// m_DeviceContext->PSSetShader( pPixelShader.Get(), nullptr, 0u );

		// Now use temp wide string for Vertex shader source input.
		ws.assign( vertexSrc.begin(), vertexSrc.end() );
		// Get temp LPCWSTR (said to be safe). 
		LPCWSTR vs = ws.c_str();

		// Create vertex shader.
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		D3DReadFileToBlob( vs, &pBlob );
		m_Device->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader );

		m_PixelShader = pPixelShader.Get();
		m_VertexShader = pVertexShader.Get();
		// m_InputLayout = pInputLayout.Get();
	}

	Shader::~Shader()
	{
	}

	void Shader::Bind()
	{
		m_DeviceContext->PSSetShader( m_PixelShader.Get(), nullptr, 0u );
		m_DeviceContext->VSSetShader( m_VertexShader.Get(), nullptr, 0u );
	}

	void Shader::Unbind()
	{
		// Com objects destroy themselves, this function may be useless as of now. 
	}
}