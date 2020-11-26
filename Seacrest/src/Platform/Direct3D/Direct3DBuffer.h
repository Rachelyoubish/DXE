#pragma once

#include "Seacrest/Renderer/Buffer.h"
#include <wrl.h>

namespace Seacrest {

	class Direct3DVertexBuffer : public VertexBuffer
	{
	public:
		Direct3DVertexBuffer( void* vertices, uint32_t size, uint32_t sizeList );
		virtual ~Direct3DVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout( const BufferLayout& layout ) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
		//Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		//Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
		// float* vertexSize;
		UINT vertexSize;
	};

	class Direct3DIndexBuffer : public IndexBuffer
	{
	public:
		Direct3DIndexBuffer( unsigned short* indices, unsigned short count, unsigned short countList );
		virtual ~Direct3DIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};
}