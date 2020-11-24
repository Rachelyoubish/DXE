#pragma once

#include "Seacrest/Renderer/Buffer.h"
#include <wrl.h>

namespace Seacrest {

	class DirectXVertexBuffer : public VertexBuffer
	{
	public:
		DirectXVertexBuffer( void* vertices, uint32_t size, uint32_t sizeList );
		virtual ~DirectXVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
	private:
		uint32_t m_RendererID;
		//Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		//Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
		// float* vertexSize;
		UINT vertexSize;
	};

	class DirectXIndexBuffer : public IndexBuffer
	{
	public:
		DirectXIndexBuffer( uint32_t* indices, unsigned short count );
		virtual ~DirectXIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
	};
}