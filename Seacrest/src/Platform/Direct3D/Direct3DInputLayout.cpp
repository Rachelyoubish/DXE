#include "scpch.h"
#include "Direct3DInputLayout.h"
#include "Seacrest/Application.h"

namespace Seacrest {

	//static DXGI_FORMAT ShaderDataTypeToDirect3DBaseType( ShaderDataType type )
	//{
	//	switch (type)
	//	{
	//		case Seacrest::ShaderDataType::Float:		return DXGI_FORMAT_R32_FLOAT;
	//		case Seacrest::ShaderDataType::Float2:		return DXGI_FORMAT_R32G32_FLOAT;
	//		case Seacrest::ShaderDataType::Float3:		return DXGI_FORMAT_R32G32B32_FLOAT;
	//		case Seacrest::ShaderDataType::Float4:		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	//		case Seacrest::ShaderDataType::Int:		    return DXGI_FORMAT_R32_UINT;
	//		case Seacrest::ShaderDataType::Int2:		return DXGI_FORMAT_R32G32_UINT;
	//		case Seacrest::ShaderDataType::Int3:		return DXGI_FORMAT_R32G32B32_UINT;
	//		case Seacrest::ShaderDataType::Int4:		return DXGI_FORMAT_R32G32B32A32_UINT;
	//	}
	//
	//	SEACREST_CORE_ASSERT( false, "Unkown ShaderDataType!" );
	//	return DXGI_FORMAT_UNKNOWN;
	//}

	Direct3DInputLayout::Direct3DInputLayout( const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderBytecode )
	{
		Application& app = Application::Get();
		ID3D11Device* pDevice = app.GetWindow().GetGraphicsContext()->GetD3D11Device();

		pDevice->CreateInputLayout(
			layout.data(), (UINT)layout.size(),
			pVertexShaderBytecode->GetBufferPointer(),
			pVertexShaderBytecode->GetBufferSize(),
			&m_InputLayout
		);


	}
	
	void Direct3DInputLayout::Bind() const
	{
		Application& app = Application::Get();
		ID3D11DeviceContext* pDeviceContext = app.GetWindow().GetGraphicsContext()->GetD3D11DeviceContext();
		// Bind vertex layout.
		pDeviceContext->IASetInputLayout( m_InputLayout.Get() );
	}

	void Direct3DInputLayout::Unbind() const
	{
	}

	void Direct3DInputLayout::AddVertexBuffer( const std::shared_ptr<VertexBuffer>& vertexBuffer, ID3DBlob* Blob )
	{
		vertexBuffer->Bind();

		//uint32_t index = 0;
		//const auto& layout = vertexBuffer->GetLayout();

		// Input (vertex) layout (2D position only).
		//D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc = new D3D11_INPUT_ELEMENT_DESC[layout.GetElements().size()];
		//for (const auto& element : layout)
		//{
		//	inputLayoutDesc[index].SemanticName = element.Name.c_str();
		//	inputLayoutDesc[index].SemanticIndex = 0;
		//	inputLayoutDesc[index].Format = ShaderDataTypeToDirect3DBaseType( element.Type );
		//	inputLayoutDesc[index].InputSlot = 0;
		//	inputLayoutDesc[index].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		//	inputLayoutDesc[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		//	inputLayoutDesc[index].InstanceDataStepRate = 0;
		//
		//	index++;
		//}

		m_VertexBuffers.push_back( vertexBuffer );

		//delete[] inputLayoutDesc;
	}

	void Direct3DInputLayout::SetIndexBuffer( const std::shared_ptr<IndexBuffer>& indexBuffer )
	{
		// TEMP, unsure if this will be used... input layout
		// far more relevent to the Vertex buffer.
		m_IndexBuffer = indexBuffer;
		indexBuffer->Bind();
	}

}
