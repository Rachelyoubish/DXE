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
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
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

		// Bind vertex shader.
		// m_DeviceContext->VSSetShader( pVertexShader.Get(), nullptr, 0u );

		// Input (vertex) layout (2D position only).
		// Warning: I'm not sure setting the Input layout is
		// best here since the function input is relevant to
		// shader creation specifically, however, this is
		// relevant to shader /data/... 
		// Update: Yeah, this should be set through the
		// D3D Buffer class.
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		m_Device->CreateInputLayout(
			ied, (UINT)std::size( ied ),
			pBlob->GetBufferPointer(),
			pBlob->GetBufferSize(),
			&pInputLayout
		);

		// Bind vertex layout.
		m_DeviceContext->IASetInputLayout( pInputLayout.Get() );

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
		// m_DeviceContext->IASetInputLayout( m_InputLayout.Get() );
		// I don't think it's logical to set the input layout every frame. 
	}

	void Shader::Unbind()
	{
		// Com objects destroy themselves, this function may be useless as of now. 
	}
}