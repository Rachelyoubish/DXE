#include "scpch.h"
#include "DirectXBuffer.h"

#include "Seacrest/Application.h"

namespace Seacrest {

	///////////////////////////////////////////////////////////////////////////
	// Vertex Buffer //////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////
	DirectXVertexBuffer::DirectXVertexBuffer( float* vertices, uint32_t size )
	{
		Application& app = Application::Get();
		ID3D11Device* pDevice = app.GetWindow().GetGraphicsContext()->GetD3D11Device();

		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

		D3D11_BUFFER_DESC bd = {};

		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = (UINT)size;
		bd.StructureByteStride = (UINT)(size);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices;

		pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
		
		m_VertexBuffer = pVertexBuffer;
		
		Bind();
	}

	DirectXVertexBuffer::~DirectXVertexBuffer()
	{

	}

	void DirectXVertexBuffer::Bind() const
	{
		Application& app = Application::Get();
		ID3D11DeviceContext* pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
		// Bind vertex buffer to pipeline
		const UINT stride = 6 * sizeof(float);
		const UINT offset = 0u;
		pDeviceContext->IASetVertexBuffers(0u, 1u, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	}
	
	void DirectXVertexBuffer::Unbind() const
	{

	}

	///////////////////////////////////////////////////////////////////////////
	// Index Buffer ///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	DirectXIndexBuffer::DirectXIndexBuffer( uint32_t* indices, unsigned short count )
		: m_Count( count )
	{
		Application& app = Application::Get();
		ID3D11Device* pDevice = app.GetWindow().GetGraphicsContext()->GetD3D11Device();
	

		// Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

		D3D11_BUFFER_DESC ibd = {};

		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = UINT(m_Count * 3);
		ibd.StructureByteStride = sizeof( unsigned short );

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices;
		pDevice->CreateBuffer( &ibd, &isd, &m_IndexBuffer );

		// m_IndexBuffer = pIndexBuffer;
		Bind();
	}

	DirectXIndexBuffer::~DirectXIndexBuffer()
	{
	}

	void DirectXIndexBuffer::Bind() const
	{
		Application& app = Application::Get();
		ID3D11DeviceContext* pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
		// Bind index buffer.
		pDeviceContext->IASetIndexBuffer( m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u );
	}

	void DirectXIndexBuffer::Unbind() const
	{
	}
}