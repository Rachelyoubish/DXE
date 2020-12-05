#include "scpch.h"
#include "Direct3DBuffer.h"

#include "Seacrest/Application.h"

namespace Seacrest {

	///////////////////////////////////////////////////////////////////////////
	// Vertex Buffer //////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	Direct3DVertexBuffer::Direct3DVertexBuffer( void* vertices, uint32_t size, uint32_t sizeList )
	{
		Application& app = Application::Get();
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice = app.GetWindow().GetGraphicsContext()->GetD3D11Device();

		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

		D3D11_BUFFER_DESC bd = {};
		SecureZeroMemory( &bd, sizeof( bd ) );

		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = size * sizeList;

		D3D11_SUBRESOURCE_DATA sd = {};
		SecureZeroMemory( &sd, sizeof( sd ) );
		sd.pSysMem = vertices;

		pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
		
		vertexSize = size;
		m_VertexBuffer = pVertexBuffer;
	}

	Direct3DVertexBuffer::~Direct3DVertexBuffer()
	{
	}

	void Direct3DVertexBuffer::Bind() const
	{
		Application& app = Application::Get();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
		// Bind vertex buffer to pipeline
		const UINT stride = vertexSize; 
		const UINT offset = 0u;
		pDeviceContext->IASetVertexBuffers(0u, 1u, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	}
	
	void Direct3DVertexBuffer::Unbind() const
	{

	}

	///////////////////////////////////////////////////////////////////////////
	// Index Buffer ///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	Direct3DIndexBuffer::Direct3DIndexBuffer( unsigned short* indices, unsigned short count, unsigned short countList )
		: m_Count( countList )
	{
		Application& app = Application::Get();
		Microsoft::WRL::ComPtr<ID3D11Device> pDevice = app.GetWindow().GetGraphicsContext()->GetD3D11Device();
	
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

		D3D11_BUFFER_DESC ibd = {};
		SecureZeroMemory( &ibd, sizeof( ibd ) );

		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = UINT(m_Count) * countList;

		D3D11_SUBRESOURCE_DATA isd = {};
		SecureZeroMemory( &isd, sizeof( isd ) );

		isd.pSysMem = indices;
		pDevice->CreateBuffer( &ibd, &isd, &pIndexBuffer );

		m_IndexBuffer = pIndexBuffer;
	}

	Direct3DIndexBuffer::~Direct3DIndexBuffer()
	{
	}

	void Direct3DIndexBuffer::Bind() const
	{
		Application& app = Application::Get();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
		// Bind index buffer.
		pDeviceContext->IASetIndexBuffer( m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u );
	}

	void Direct3DIndexBuffer::Unbind() const
	{
	}
}