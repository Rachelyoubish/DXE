#pragma once

#include "Seacrest/Renderer/InputLayout.h"
#include "Seacrest/Renderer/Shader.h"

namespace Seacrest {

	class Direct3DInputLayout : public InputLayout
	{
	public:
		Direct3DInputLayout();
		virtual ~Direct3DInputLayout() = default;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer( const std::shared_ptr<VertexBuffer>& vertexBuffer, ID3DBlob* Blob ) override;
		virtual void SetIndexBuffer( const std::shared_ptr<IndexBuffer>& indexBuffer ) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<Shader> m_Shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
		HRESULT hr;
	};
}