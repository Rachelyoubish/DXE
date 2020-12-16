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

		virtual void AddVertexBuffer( const Ref<VertexBuffer>& vertexBuffer, ID3DBlob* Blob ) override;
		virtual void SetIndexBuffer( const Ref<IndexBuffer>& indexBuffer ) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Shader> m_Shader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	};
}