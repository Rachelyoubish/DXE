#pragma once

namespace Seacrest {

	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		static VertexBuffer* Create( void* vertices, uint32_t size, uint32_t sizeList );
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create( unsigned short* indices, unsigned short size, unsigned short sizeList);
	};
}