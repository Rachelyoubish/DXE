#include "scpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/Direct3D/Direct3DBuffer.h"

namespace Seacrest {

	VertexBuffer* VertexBuffer::Create( void* vertices, uint32_t size, uint32_t sizeList )
	{
		switch (Renderer::GetCurrentAPI())
		{
			case RendererAPI::None:     SEACREST_ASSERT(false, "RendererAPI::None is currently not supported!");  return nullptr;
			case RendererAPI::Direct3D: return new Direct3DVertexBuffer(vertices, size, sizeList);
		}

		SEACREST_ASSERT( false, "Unknown RendererAPI!" );
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create( unsigned short* indices, unsigned short size, unsigned short sizeList)
	{
		switch (Renderer::GetCurrentAPI())
		{
			case RendererAPI::None:     SEACREST_ASSERT( false, "RendererAPI::None is currently not supported!" );  return nullptr;
			case RendererAPI::Direct3D: return new Direct3DIndexBuffer( indices, size, sizeList );
		}

		SEACREST_ASSERT( false, "Unknown RendererAPI!" );
		return nullptr;
	}
}
