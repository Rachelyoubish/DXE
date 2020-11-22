#pragma once

namespace Seacrest {

	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		static VertexBuffer* Create( const std::vector<float>& vertices, uint32_t size );
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create( const std::vector<unsigned short>& indicies, unsigned short size);
	};
}