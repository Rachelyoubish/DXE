#include "scpch.h"
#include "Direct3DShader.h"
#include "Seacrest/Application.h"
#include <d3d11shader.h>

namespace Seacrest {

	Direct3DShader::Direct3DShader( const std::string& vertexSrc, const std::string& pixelSrc )
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
	}

	Direct3DShader::~Direct3DShader()
	{
	}

	void Direct3DShader::Bind() const 
	{
		m_DeviceContext->PSSetShader( m_PixelShader.Get(), nullptr, 0u );
		m_DeviceContext->VSSetShader( m_VertexShader.Get(), nullptr, 0u );
	}

	void Direct3DShader::Unbind() const
	{
		// Com objects destroy themselves, this function may be useless as of now. 
	}

	void Direct3DShader::UploadConstantFloat4( const std::string& name, const DirectX::XMVECTOR& value )
	{
		
		if(!m_ConstantBufferColor )
		{
			D3D11_BUFFER_DESC cbDesc = { 0 };
			SecureZeroMemory( &cbDesc, sizeof( cbDesc ) );

			cbDesc.ByteWidth = sizeof( DirectX::XMVECTOR );
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA InitData = { 0 };
			SecureZeroMemory( &InitData, sizeof( InitData ) );

			InitData.pSysMem = &value;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			m_Device->CreateBuffer( &cbDesc, &InitData, &m_ConstantBufferColor );
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		m_DeviceContext->Map( m_ConstantBufferColor.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms );
		memcpy( ms.pData, &value, sizeof( DirectX::XMVECTOR ) );
		m_DeviceContext->Unmap( m_ConstantBufferColor.Get(), NULL );

		m_DeviceContext->VSSetConstantBuffers( 2, 1, m_ConstantBufferColor.GetAddressOf() );
	}

	void Direct3DShader::UploadConstantMat( const std::string& name, const DirectX::XMMATRIX& matrix )
	{
		if (!m_ConstantBuffer)
		{
			D3D11_BUFFER_DESC cbDesc = { 0 };
			SecureZeroMemory( &cbDesc, sizeof( cbDesc ) );

			cbDesc.ByteWidth = sizeof( DirectX::XMMATRIX );
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA InitData = { 0 };
			SecureZeroMemory( &InitData, sizeof( InitData ) );

			InitData.pSysMem = &matrix;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			m_Device->CreateBuffer( &cbDesc, &InitData, &m_ConstantBuffer );
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		m_DeviceContext->Map( m_ConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms );
		memcpy( ms.pData, &matrix, sizeof( DirectX::XMMATRIX ) );
		m_DeviceContext->Unmap( m_ConstantBuffer.Get(), NULL );

		m_DeviceContext->VSSetConstantBuffers( 0, 1, m_ConstantBuffer.GetAddressOf() );
	}

	void Direct3DShader::UploadConstantTransform( const std::string& name, const DirectX::XMMATRIX& matrix )
	{
		if (!m_ConstantBufferTransform)
		{
			D3D11_BUFFER_DESC cbDesc = { 0 };
			SecureZeroMemory( &cbDesc, sizeof( cbDesc ) );

			cbDesc.ByteWidth = sizeof( DirectX::XMMATRIX );
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA InitData = { 0 };
			SecureZeroMemory( &InitData, sizeof( InitData ) );

			InitData.pSysMem = &matrix;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			m_Device->CreateBuffer( &cbDesc, &InitData, &m_ConstantBufferTransform );
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		m_DeviceContext->Map( m_ConstantBufferTransform.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms );
		memcpy( ms.pData, &matrix, sizeof( DirectX::XMMATRIX ) );
		m_DeviceContext->Unmap( m_ConstantBufferTransform.Get(), NULL );

		m_DeviceContext->VSSetConstantBuffers( 1, 1, m_ConstantBufferTransform.GetAddressOf() );
	}
}
