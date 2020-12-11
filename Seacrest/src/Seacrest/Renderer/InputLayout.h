#pragma once

#include <memory>
#include <vector>
#include "Seacrest/Renderer/Buffer.h"

namespace Seacrest {

	class InputLayout
	{
		// Preserve Buffer layout to keep API intact, 
		// while getting rid of D3D code in application...
		// Thus a special class just to abstract the Input
		// Layout is created. 
	public:
		virtual ~InputLayout() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer( const Ref<VertexBuffer>& vertexBuffer, ID3DBlob* Blob ) = 0;
		virtual void SetIndexBuffer( const Ref<IndexBuffer>& indexBuffer ) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static InputLayout* Create();
	};
}