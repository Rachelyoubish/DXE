#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Seacrest {

	class Renderer
	{
	public:
		static void Init();

		static void BeginScene( OrthographicCamera& camera ); 
		static void EndScene();

		static void Submit( const Ref<Shader>& shader, const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer, const DirectX::XMMATRIX& transform = DirectX::XMMatrixIdentity() );

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			DirectX::XMMATRIX ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}